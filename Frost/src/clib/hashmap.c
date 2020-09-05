#include "hashmap.h"

#include "clib.h"

#include "hash.h"

/* Table sizes must be powers of 2 */
#define CLIB_HASHMAP_SIZE_MIN               (1 << 5)    /* 32 */
#define CLIB_HASHMAP_SIZE_DEFAULT           (1 << 8)    /* 256 */
#define CLIB_HASHMAP_SIZE_MOD(map, val)     ((val) & ((map)->capacity - 1))

/* Limit for probing is 1/2 of table_size */
#define CLIB_HASHMAP_PROBE_LEN(map)         ((map)->capacity >> 1)
/* Return the next linear probe index */
#define CLIB_HASHMAP_PROBE_NEXT(map, index) CLIB_HASHMAP_SIZE_MOD(map, (index) + 1)

/* Check if index b is less than or equal to index a */
#define CLIB_HASHMAP_INDEX_LESS(map, a, b)  ((a) == (b) || (((b) - (a)) & ((map)->capacity >> 1)) != 0)

/*
 * Enforce a maximum 0.75 load factor.
 */
static inline size_t clib_hashmap_table_calc_min_size(size_t num_entries)
{
    return num_entries + (num_entries / 3);
}

/*
 * Calculate the optimal table size, given the specified max number of elements.
 */
static size_t clib_hashmap_table_calc_size(size_t num_entries)
{
    size_t table_size = clib_hashmap_table_calc_min_size(num_entries);

    /* Table size is always a power of 2 */
    size_t min_size = CLIB_HASHMAP_SIZE_MIN;
    while (min_size < table_size)
        min_size <<= 1;

    return min_size;
}

/*
 * Get a valid hash table index from a key.
 */
static inline size_t clib_hashmap_calc_index(const clib_hashmap* map, const void* key)
{
    return CLIB_HASHMAP_SIZE_MOD(map, map->hash(key));
}

/*
 * Return the next populated entry, starting with the specified one.
 * Returns NULL if there are no more valid entries.
 */
static clib_hashmap_entry* clib_hashmap_entry_get_populated(const clib_hashmap* map, clib_hashmap_entry* entry)
{
    for (; entry < &map->table[map->capacity]; ++entry)
    {
        if (entry->key)
            return entry;
    }
    return NULL;
}

clib_hashmap_error clib_hashmap_alloc(clib_hashmap* map, size_t (*hash_func)(const void*), int (*cmp_func)(const void*, const void*), size_t initial_capacity)
{
    CLIB_ASSERT(map != NULL);
    CLIB_ASSERT(hash_func != NULL);
    CLIB_ASSERT(cmp_func != NULL);

    if (!initial_capacity)
        map->capacity = CLIB_HASHMAP_SIZE_DEFAULT;
    else
        /* Convert init size to valid table size */
        map->capacity = clib_hashmap_table_calc_size(initial_capacity); 
    
    map->table = calloc(map->capacity, sizeof(clib_hashmap_entry));
    map->used = 0;

    if (!map->table)
        return CLIB_HASHMAP_ALLOC_ERROR;

    map->hash = hash_func;
    map->key_cmp = cmp_func;

    map->key_alloc = NULL;
    map->key_free = NULL;

    map->value_alloc = NULL;
    map->value_free = NULL;

    return CLIB_HASHMAP_OK;
}

static void clib_hashmap_free_keys(clib_hashmap* map)
{
    if (!map->key_free)
        return;

    for (clib_hashmap_iter* iter = clib_hashmap_iterator(map); iter; iter = clib_hashmap_iter_next(map, iter))
        map->key_free((void*)clib_hashmap_iter_get_key(iter));
}

static void clib_hashmap_free_values(clib_hashmap* map)
{
    if (!map->value_free)
        return;

    for (clib_hashmap_iter* iter = clib_hashmap_iterator(map); iter; iter = clib_hashmap_iter_next(map, iter))
        map->value_free((void*)clib_hashmap_iter_get_value(iter));
}

void clib_hashmap_free(clib_hashmap* map)
{
    if (!map) return;

    clib_hashmap_free_keys(map);
    clib_hashmap_free_values(map);
    free(map->table);
    map->capacity = CLIB_HASHMAP_SIZE_DEFAULT;
    map->used = 0;
}

void clib_hashmap_set_key_alloc_funcs(clib_hashmap* map, void* (*alloc_func)(const void*), void (*free_func)(void*))
{
    CLIB_ASSERT(map != NULL);

    map->key_alloc = alloc_func;
    map->key_free = free_func;
}

void clib_hashmap_set_value_alloc_funcs(clib_hashmap* map, void* (*alloc_func)(const void*), void (*free_func)(void*))
{
    CLIB_ASSERT(map != NULL);

    map->value_alloc = alloc_func;
    map->value_free = free_func;
}

clib_hashmap_error clib_hashmap_rehash(clib_hashmap* map, size_t new_capacity)
{
    CLIB_ASSERT(new_capacity >= CLIB_HASHMAP_SIZE_MIN);
    CLIB_ASSERT((new_capacity & (new_capacity - 1)) == 0);

    clib_hashmap_entry* new_table = calloc(new_capacity, sizeof(clib_hashmap_entry));
    if (!new_table) return CLIB_HASHMAP_ALLOC_ERROR;

    /* Backup old elements in case of rehash failure */
    size_t old_capacity = map->capacity;
    clib_hashmap_entry* old_table = map->table;

    map->capacity = new_capacity;
    map->table = new_table;

    /* Rehash */
    for (clib_hashmap_entry* entry = old_table; entry < &old_table[old_capacity]; ++entry)
    {
        if (!entry->value) continue; /* Only copy entries with value */

        clib_hashmap_entry* new_entry = clib_hashmap_entry_find(map, entry->key, 1);
        if (!new_entry)
        {
            /*
             * The load factor is too high with the new table size,
             * or a poor hash function was used. 
             */
            map->capacity = old_capacity;
            map->table = old_table;
            free(new_table);
            return CLIB_HASHMAP_HASH_ERROR;
        }

        /* Shallow copy */
        new_entry->key = entry->key;
        new_entry->value = entry->value;
    }

    free(old_table);
    return CLIB_HASHMAP_OK;
}

void* clib_hashmap_insert(clib_hashmap* map, const void* key, void* value)
{
    CLIB_ASSERT(map != NULL);

    /* Rehash with 2x capacity if load factor is approaching 0.75 */
    if (map->capacity <= clib_hashmap_table_calc_min_size(map->used))
        clib_hashmap_rehash(map, map->capacity << 1);

    clib_hashmap_entry* entry = clib_hashmap_entry_find(map, key, 1);
    if (!entry)
    {
        /*
         * Cannot find an empty slot.  Either out of memory, or using
         * a poor hash function.  Attempt to rehash once to reduce
         * chain length.
         */
        if (clib_hashmap_rehash(map, map->capacity << 1) != CLIB_HASHMAP_OK)
            return NULL;

        entry = clib_hashmap_entry_find(map, key, 1);
        if (!entry)
            return NULL;
    }
    if (!entry->key) 
    {
        /* Allocate copy of key to simplify memory management */
        if (map->key_alloc)
        {
            entry->key = map->key_alloc(key);
            if (!entry->key)
                return NULL;
        } 
        else 
        {
            entry->key = (void*)key;
        }
        ++map->used;
    }
    else if (entry->value)
    {
        /* Do not overwrite existing value */
        return entry->value;
    }

    if (map->value_alloc)
        entry->value = map->value_alloc(value);
    else
        entry->value = value;
    
    return value;
}

clib_hashmap_error clib_hashmap_remove(clib_hashmap* map, const void *key)
{
    CLIB_ASSERT(map != NULL);
    CLIB_ASSERT(key != NULL);

    clib_hashmap_entry* entry = clib_hashmap_entry_find(map, key, 0);
    if (entry)
    {
        /* Clear the entry and make the chain contiguous */
        clib_hashmap_entry_remove(map, entry);
        return CLIB_HASHMAP_OK;
    }
    return CLIB_HASHMAP_KEY_NOT_FOUND;
}

void clib_hashmap_clear(clib_hashmap* map)
{
    CLIB_ASSERT(map != NULL);

    clib_hashmap_free_keys(map);
    map->used = 0;
    memset(map->table, 0, sizeof(clib_hashmap_entry) * map->capacity);
}

void* clib_hashmap_find(const clib_hashmap* map, const void* key)
{
    CLIB_ASSERT(map != NULL);

    clib_hashmap_entry* entry = clib_hashmap_entry_find(map, key, 0);

    if (!entry) return NULL;

    return entry->value;
}

const void* clib_hashmap_get_key_ptr(const clib_hashmap* map, const void* key)
{
    CLIB_ASSERT(map != NULL);
    CLIB_ASSERT(key != NULL);

    clib_hashmap_entry* entry = clib_hashmap_entry_find(map, key, 0);

    if (!entry) return NULL;

    return entry->key;
}

clib_hashmap_entry* clib_hashmap_entry_find(const clib_hashmap* map, const void* key, int find_empty)
{
    size_t probe_len = CLIB_HASHMAP_PROBE_LEN(map);
    size_t index = clib_hashmap_calc_index(map, key);

    /* Linear probing */
    for (size_t i = 0; i < probe_len; ++i)
    {
        clib_hashmap_entry* entry = &map->table[index];
        if (!entry->key)
        {
            if (find_empty)
                return entry;

            return NULL;
        }

        if (map->key_cmp(key, entry->key) == 0)
            return entry;

        index = CLIB_HASHMAP_PROBE_NEXT(map, index);
    }
    return NULL;
}

void clib_hashmap_entry_remove(clib_hashmap* map, clib_hashmap_entry* removed_entry)
{
    size_t removed_index = (removed_entry - map->table);

    /* free memory */
    if (map->key_free)
        map->key_free(removed_entry->key);
    if (map->value_free)
        map->value_free(removed_entry->value);

    --map->used;

    /* Fill the free slot in the chain */
    size_t index = CLIB_HASHMAP_PROBE_NEXT(map, removed_index);
    for (size_t i = 1; i < map->capacity; ++i)
    {
        clib_hashmap_entry* entry = &map->table[index];
        if (!entry->key)
            break; /* Reached end of chain */

        size_t entry_index = clib_hashmap_calc_index(map, entry->key);
        /* Shift in entries with an index <= to the removed slot */
        if (CLIB_HASHMAP_INDEX_LESS(map, removed_index, entry_index))
        {
            memcpy(removed_entry, entry, sizeof(*removed_entry));
            removed_index = index;
            removed_entry = entry;
        }
        index = CLIB_HASHMAP_PROBE_NEXT(map, index);
    }
    /* Clear the last removed entry */
    memset(removed_entry, 0, sizeof(*removed_entry));
}

clib_hashmap_iter* clib_hashmap_iterator(const clib_hashmap* map)
{
    CLIB_ASSERT(map != NULL);

    if (!map->used)
        return NULL;

    return (clib_hashmap_iter*)clib_hashmap_entry_get_populated(map, map->table);
}

clib_hashmap_iter* clib_hashmap_iter_next(const clib_hashmap* map, const clib_hashmap_iter* iter)
{
    CLIB_ASSERT(map != NULL);

    if (!iter) return NULL;

    clib_hashmap_entry* entry = (clib_hashmap_entry*)iter;

    return (clib_hashmap_iter*)clib_hashmap_entry_get_populated(map, entry + 1);
}

clib_hashmap_iter* clib_hashmap_iter_remove(clib_hashmap* map, const clib_hashmap_iter* iter)
{
    CLIB_ASSERT(map != NULL);

    if (!iter)
        return NULL;

    clib_hashmap_entry* entry = (clib_hashmap_entry*)iter;

    if (!entry->key)
        return clib_hashmap_iter_next(map, iter); /* Iterator is invalid, so just return the next valid entry */

    clib_hashmap_entry_remove(map, entry);
    return (clib_hashmap_iter*)clib_hashmap_entry_get_populated(map, entry);
}

const void* clib_hashmap_iter_get_key(const clib_hashmap_iter* iter)
{
    if (!iter)
        return NULL;

    return (const void*)((clib_hashmap_entry*)iter)->key;
}

void* clib_hashmap_iter_get_value(const clib_hashmap_iter* iter)
{
    if (!iter)
        return NULL;

    return ((clib_hashmap_entry*)iter)->value;
}

int clib_hashmap_str_cmp(const void* a, const void* b)
{
    return strcmp((const char*)a, (const char*)b);
}

void* clib_hashmap_str_alloc(const void* src)
{
    size_t size = strlen(src);
    char* dst = malloc(size + 1);

    if (!dst) return NULL;

    strcpy(dst, src);
    /* make sure string is null-terminated */
    dst[size] = '\0';

    return (void*)dst;
}

void clib_hashmap_str_free(void* block)
{
    free(block);
}

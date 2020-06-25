#include "hashset.h"

#include "clib.h"

/* Table sizes must be powers of 2 */
#define CLIB_HASHSET_SIZE_MIN               (1 << 5)    /* 32 */
#define CLIB_HASHSET_SIZE_DEFAULT           (1 << 8)    /* 256 */
#define CLIB_HASHSET_SIZE_MOD(set, val)     ((val) & ((set)->capacity - 1))

/* Limit for probing is 1/2 of table_size */
#define CLIB_HASHSET_PROBE_LEN(set)         ((set)->capacity >> 1)
/* Return the next linear probe index */
#define CLIB_HASHSET_PROBE_NEXT(set, index) CLIB_HASHSET_SIZE_MOD(set, (index) + 1)

/* Check if index b is less than or equal to index a */
#define CLIB_HASHSET_INDEX_LESS(set, a, b)  ((a) == (b) || (((b) - (a)) & ((set)->capacity >> 1)) != 0)

/*
 * Enforce a maximum 0.75 load factor.
 */
static inline size_t clib_hashset_table_calc_min_size(size_t num_entries)
{
    return num_entries + (num_entries / 3);
}

/*
 * Calculate the optimal table size, given the specified max number of elements.
 */
static size_t clib_hashset_table_calc_size(size_t num_entries)
{
    size_t table_size = clib_hashset_table_calc_min_size(num_entries);

    /* Table size is always a power of 2 */
    size_t min_size = CLIB_HASHSET_SIZE_MIN;
    while (min_size < table_size)
        min_size <<= 1;

    return min_size;
}

/*
 * Get a valid hash table index from a key.
 */
static inline size_t clib_hashset_calc_index(const clib_hashset* set, int32_t key)
{
    return CLIB_HASHSET_SIZE_MOD(set, set->hash(key));
}

/*
 * Return the next populated entry, starting with the specified one.
 * Returns NULL if there are no more valid entries.
 */
static clib_hashset_entry* clib_hashset_entry_get_populated(const clib_hashset* set, clib_hashset_entry* entry)
{
    while (entry < &set->table[set->capacity])
    {
        if (entry->key >= 0)
            return entry;
        ++entry;
    }
    return NULL;
}

static clib_hashset_entry* clib_entry_calloc(size_t count, size_t size)
{
    clib_hashset_entry* table = malloc(count * size);

    for (size_t i = 0; i < count; ++i)
    {
        table[i].key = -1;
        table[i].value = NULL;
    }

    return table;
}

clib_hashset_error clib_hashset_alloc(clib_hashset* set, size_t(*hash_func)(int32_t), size_t initial_capacity)
{
    CLIB_ASSERT(set != NULL);
    CLIB_ASSERT(hash_func != NULL);

    /* Convert init size to valid table size */
    set->capacity = initial_capacity ? clib_hashset_table_calc_size(initial_capacity) : CLIB_HASHSET_SIZE_DEFAULT;

    set->table = clib_entry_calloc(set->capacity, sizeof(clib_hashset_entry));
    set->used = 0;

    if (!set->table)
        return CLIB_HASHSET_ALLOC_ERROR;

    set->hash = hash_func;

    set->value_alloc = NULL;
    set->value_free = NULL;

    return CLIB_HASHSET_OK;
}

static void clib_hashset_free_values(clib_hashset* set)
{
    if (!set->value_free)
        return;

    for (clib_hashset_iter* iter = clib_hashset_iterator(set); iter; iter = clib_hashset_iter_next(set, iter))
        set->value_free((void*)clib_hashset_iter_get_value(iter));
}

void clib_hashset_free(clib_hashset* set)
{
    if (!set) return;

    clib_hashset_free_values(set);
    free(set->table);
    set->capacity = CLIB_HASHSET_SIZE_DEFAULT;
    set->used = 0;
}

void clib_hashset_set_value_alloc_funcs(clib_hashset* set, void* (*alloc_func)(const void*), void (*free_func)(void*))
{
    CLIB_ASSERT(set != NULL);

    set->value_alloc = alloc_func;
    set->value_free = free_func;
}

clib_hashset_error clib_hashset_rehash(clib_hashset* set, size_t new_capacity)
{
    CLIB_ASSERT(new_capacity >= CLIB_HASHSET_SIZE_MIN);
    CLIB_ASSERT((new_capacity & (new_capacity - 1)) == 0);

    clib_hashset_entry* new_table = clib_entry_calloc(new_capacity, sizeof(clib_hashset_entry));
    if (!new_table) return CLIB_HASHSET_ALLOC_ERROR;

    /* Backup old elements in case of rehash failure */
    size_t old_capacity = set->capacity;
    clib_hashset_entry* old_table = set->table;

    set->capacity = new_capacity;
    set->table = new_table;

    /* Rehash */
    for (clib_hashset_entry* entry = old_table; entry < &old_table[old_capacity]; ++entry)
    {
        if (!entry->value) continue; /* Only copy entries with value */

        clib_hashset_entry* new_entry = clib_hashset_entry_find(set, entry->key, 1);
        if (!new_entry)
        {
            /*
             * The load factor is too high with the new table size,
             * or a poor hash function was used.
             */
            set->capacity = old_capacity;
            set->table = old_table;
            free(new_table);
            return CLIB_HASHSET_HASH_ERROR;
        }

        /* Shallow copy */
        new_entry->key = entry->key;
        new_entry->value = entry->value;
    }

    free(old_table);
    return CLIB_HASHSET_OK;
}

void* clib_hashset_insert(clib_hashset* set, int32_t key, void* value)
{
    CLIB_ASSERT(set != NULL);

    /* Rehash with 2x capacity if load factor is approaching 0.75 */
    if (set->capacity <= clib_hashset_table_calc_min_size(set->used))
        clib_hashset_rehash(set, set->capacity << 1);

    clib_hashset_entry* entry = clib_hashset_entry_find(set, key, 1);
    if (!entry)
    {
        /*
         * Cannot find an empty slot.  Either out of memory, or using
         * a poor hash function.  Attempt to rehash once to reduce
         * chain length.
         */
        if (clib_hashset_rehash(set, set->capacity << 1) != CLIB_HASHSET_OK)
            return NULL;

        entry = clib_hashset_entry_find(set, key, 1);
        if (!entry)
            return NULL;
    }
    if (entry->key < 0)
    {
        entry->key = key;
        ++set->used;
    }
    else if (entry->value)
    {
        /* Do not overwrite existing value */
        return entry->value;
    }

    if (set->value_alloc)
        entry->value = set->value_alloc(value);
    else
        entry->value = value;

    return value;
}

clib_hashset_error clib_hashset_remove(clib_hashset* set, int32_t key)
{
    CLIB_ASSERT(set != NULL);

    clib_hashset_entry* entry = clib_hashset_entry_find(set, key, 0);
    if (entry)
    {
        /* Clear the entry and make the chain contiguous */
        clib_hashset_entry_remove(set, entry);
        return CLIB_HASHSET_OK;
    }
    return CLIB_HASHSET_KEY_NOT_FOUND;
}

void clib_hashset_clear(clib_hashset* set)
{
    CLIB_ASSERT(set != NULL);

    clib_hashset_free_values(set);
    set->used = 0;
    memset(set->table, 0, sizeof(clib_hashset_entry) * set->capacity);
}

void* clib_hashset_find(const clib_hashset* set, int32_t key)
{
    CLIB_ASSERT(set != NULL);

    clib_hashset_entry* entry = clib_hashset_entry_find(set, key, 0);

    if (!entry) return NULL;

    return entry->value;
}

clib_hashset_entry* clib_hashset_entry_find(const clib_hashset* set, int32_t key, int find_empty)
{
    size_t probe_len = CLIB_HASHSET_PROBE_LEN(set);
    size_t index = clib_hashset_calc_index(set, key);

    /* Linear probing */
    for (size_t i = 0; i < probe_len; ++i)
    {
        clib_hashset_entry* entry = &set->table[index];
        if (entry->key < 0)
        {
            if (find_empty)
                return entry;

            return NULL;
        }

        if (key == entry->key)
            return entry;

        index = CLIB_HASHSET_PROBE_NEXT(set, index);
    }
    return NULL;
}

void clib_hashset_entry_remove(clib_hashset* set, clib_hashset_entry* removed_entry)
{
    size_t removed_index = (removed_entry - set->table);

    /* free memory */
    if (set->value_free)
        set->value_free(removed_entry->value);

    --set->used;

    /* Fill the free slot in the chain */
    size_t index = CLIB_HASHSET_PROBE_NEXT(set, removed_index);
    for (size_t i = 1; i < set->capacity; ++i)
    {
        clib_hashset_entry* entry = &set->table[index];
        if (entry->key < 0)
            break; /* Reached end of chain */

        size_t entry_index = clib_hashset_calc_index(set, entry->key);
        /* Shift in entries with an index <= to the removed slot */
        if (CLIB_HASHSET_INDEX_LESS(set, removed_index, entry_index))
        {
            memcpy(removed_entry, entry, sizeof(*removed_entry));
            removed_index = index;
            removed_entry = entry;
        }
        index = CLIB_HASHSET_PROBE_NEXT(set, index);
    }
    /* Clear the last removed entry */
    memset(removed_entry, 0, sizeof(*removed_entry));
}

clib_hashset_iter* clib_hashset_iterator(const clib_hashset* set)
{
    CLIB_ASSERT(set != NULL);

    if (!set->used)
        return NULL;

    return (clib_hashset_iter*)clib_hashset_entry_get_populated(set, set->table);
}

clib_hashset_iter* clib_hashset_iter_next(const clib_hashset* set, const clib_hashset_iter* iter)
{
    CLIB_ASSERT(set != NULL);

    if (!iter) return NULL;

    clib_hashset_entry* entry = (clib_hashset_entry*)iter;

    return (clib_hashset_iter*)clib_hashset_entry_get_populated(set, entry + 1);
}

clib_hashset_iter* clib_hashset_iter_remove(clib_hashset* set, const clib_hashset_iter* iter)
{
    CLIB_ASSERT(set != NULL);

    if (!iter)
        return NULL;

    clib_hashset_entry* entry = (clib_hashset_entry*)iter;

    if (!entry->key)
        return clib_hashset_iter_next(set, iter); /* Iterator is invalid, so just return the next valid entry */

    clib_hashset_entry_remove(set, entry);
    return (clib_hashset_iter*)clib_hashset_entry_get_populated(set, entry);
}

int32_t clib_hashset_iter_get_key(const clib_hashset_iter* iter)
{
    if (!iter)
        return 0;

    return ((clib_hashset_entry*)iter)->key;
}

void* clib_hashset_iter_get_value(const clib_hashset_iter* iter)
{
    if (!iter)
        return NULL;

    return ((clib_hashset_entry*)iter)->value;
}
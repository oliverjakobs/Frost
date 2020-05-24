/*
 * Copyright (c) 2016-2018 David Leeds <davidesleeds@gmail.com>
 *
 * Hashmap is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */
#include "hashmap.h"

#include "clib.h"

/* Table sizes must be powers of 2 */
#define CLIB_HASHMAP_SIZE_MIN               (1 << 5)    /* 32 */
#define CLIB_HASHMAP_SIZE_DEFAULT           (1 << 8)    /* 256 */
#define CLIB_HASHMAP_SIZE_MOD(map, val)     ((val) & ((map)->capacity - 1))

/* Limit for probing is 1/2 of table_size */
#define CLIB_HASHMAP_PROBE_LEN(map)         ((map)->capacity >> 1)
/* Return the next linear probe index */
#define CLIB_HASHMAP_PROBE_NEXT(map, index) CLIB_HASHMAP_SIZE_MOD(map, (index) + 1)

/* Check if index b is less than or equal to index a */
#define CLIB_HASHMAP_INDEX_LE(map, a, b)    ((a) == (b) || (((b) - (a)) & ((map)->capacity >> 1)) != 0)

/*
 * Enforce a maximum 0.75 load factor.
 */
static inline size_t clib_hashmap_table_min_size_calc(size_t num_entries)
{
    return num_entries + (num_entries / 3);
}

/*
 * Calculate the optimal table size, given the specified max number
 * of elements.
 */
static size_t clib_hashmap_table_size_calc(size_t num_entries)
{
    size_t table_size = clib_hashmap_table_min_size_calc(num_entries);

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

/*
 * Find the hashmap entry with the specified key, or an empty slot.
 * Returns NULL if the entire table has been searched without finding a match.
 */
static clib_hashmap_entry* clib_hashmap_entry_find(const clib_hashmap* map, const void* key, bool find_empty)
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
            {
#ifdef CLIB_HASHMAP_METRICS
                entry->num_collisions = i;
#endif
                return entry;
            }
            return NULL;
        }

        if (map->key_compare(key, entry->key) == 0)
            return entry;

        index = CLIB_HASHMAP_PROBE_NEXT(map, index);
    }
    return NULL;
}

/*
 * Removes the specified entry and processes the proceeding entries to reduce
 * the load factor and keep the chain continuous.  This is a required
 * step for hash maps using linear probing.
 */
static void clib_hashmap_entry_remove(clib_hashmap* map, clib_hashmap_entry* removed_entry)
{
    size_t removed_index = (removed_entry - map->table);

#ifdef CLIB_HASHMAP_METRICS
    size_t removed_i = 0;
#endif

    /* Free the key */
    if (map->key_free)
        map->key_free(removed_entry->key);

    --map->size;

    /* Fill the free slot in the chain */
    size_t index = CLIB_HASHMAP_PROBE_NEXT(map, removed_index);
    for (size_t i = 1; i < map->capacity; ++i)
    {
        clib_hashmap_entry* entry = &map->table[index];
        if (!entry->key)
            break; /* Reached end of chain */

        size_t entry_index = clib_hashmap_calc_index(map, entry->key);
        /* Shift in entries with an index <= to the removed slot */
        if (CLIB_HASHMAP_INDEX_LE(map, removed_index, entry_index))
        {
#ifdef CLIB_HASHMAP_METRICS
            entry->num_collisions -= (i - removed_i);
            removed_i = i;
#endif
            memcpy(removed_entry, entry, sizeof(*removed_entry));
            removed_index = index;
            removed_entry = entry;
        }
        index = CLIB_HASHMAP_PROBE_NEXT(map, index);
    }
    /* Clear the last removed entry */
    memset(removed_entry, 0, sizeof(*removed_entry));
}

/*
 * Reallocates the hash table to the new size and rehashes all entries.
 * new_size MUST be a power of 2.
 * Returns 0 on success and -errno on allocation or hash function failure.
 */
static int clib_hashmap_rehash(clib_hashmap* map, size_t new_cap)
{
    CLIB_ASSERT(new_cap >= CLIB_HASHMAP_SIZE_MIN);
    CLIB_ASSERT((new_cap & (new_cap - 1)) == 0);

    clib_hashmap_entry* new_table = (clib_hashmap_entry*)calloc(new_cap, sizeof(clib_hashmap_entry));
    if (!new_table) return -ENOMEM;

    /* Backup old elements in case of rehash failure */
    size_t old_cap = map->capacity;
    clib_hashmap_entry* old_table = map->table;
    map->capacity = new_cap;
    map->table = new_table;
    /* Rehash */
    for (clib_hashmap_entry* entry = old_table; entry < &old_table[old_cap]; ++entry)
    {
        if (!entry->value) continue; /* Only copy entries with value */

        clib_hashmap_entry* new_entry = clib_hashmap_entry_find(map, entry->key, true);
        if (!new_entry)
        {
            /*
             * The load factor is too high with the new table
             * size, or a poor hash function was used.
             */
            map->capacity = old_cap;
            map->table = old_table;
            free(new_table);
            return -EINVAL;
        }
        /* Shallow copy (intentionally omits num_collisions) */
        new_entry->key = entry->key;
        new_entry->value = entry->value;
    }
    free(old_table);
    return 0;
}

/*
 * Iterate through all entries and free all keys.
 */
static void clib_hashmap_free_keys(clib_hashmap* map)
{
    if (!map->key_free)
        return;

    for (clib_hashmap_iter* iter = clib_hashmap_iterator(map); iter; iter = clib_hashmap_iter_next(map, iter))
        map->key_free((void*)clib_hashmap_iter_get_key(iter));
}

int clib_hashmap_init(clib_hashmap* map, size_t (*hash_func)(const void*), int (*key_compare_func)(const void*, const void*), size_t initial_size)
{
    CLIB_ASSERT(map != NULL);

    if (!initial_size)
        initial_size = CLIB_HASHMAP_SIZE_DEFAULT;
    else
        initial_size = clib_hashmap_table_size_calc(initial_size); /* Convert init size to valid table size */
    
    map->capacity_init = initial_size;
    map->capacity = initial_size;
    map->size = 0;
    map->table = (clib_hashmap_entry*)calloc(initial_size, sizeof(clib_hashmap_entry));

    if (!map->table)
        return -ENOMEM;

    map->hash = hash_func ? hash_func : clib_hashmap_hash_string;
    map->key_compare = key_compare_func ? key_compare_func : clib_hashmap_compare_string;
    map->key_alloc = NULL;
    map->key_free = NULL;
    return 0;
}

void clib_hashmap_destroy(clib_hashmap* map)
{
    if (!map)
        return;

    clib_hashmap_free_keys(map);
    free(map->table);
    memset(map, 0, sizeof(*map));
}

void clib_hashmap_set_key_alloc_funcs(clib_hashmap* map, void* (*key_alloc_func)(const void*), void (*key_free_func)(void*))
{
    CLIB_ASSERT(map != NULL);

    map->key_alloc = key_alloc_func;
    map->key_free = key_free_func;
}

void* clib_hashmap_insert(clib_hashmap* map, const void* key, void* value)
{
    CLIB_ASSERT(map != NULL);
    CLIB_ASSERT(key != NULL);

    /* Rehash with 2x capacity if load factor is approaching 0.75 */
    if (map->capacity <= clib_hashmap_table_min_size_calc(map->size))
        clib_hashmap_rehash(map, map->capacity << 1);

    clib_hashmap_entry* entry = clib_hashmap_entry_find(map, key, true);
    if (!entry)
    {
        /*
         * Cannot find an empty slot.  Either out of memory, or using
         * a poor hash function.  Attempt to rehash once to reduce
         * chain length.
         */
        if (clib_hashmap_rehash(map, map->capacity << 1) < 0)
            return NULL;

        entry = clib_hashmap_entry_find(map, key, true);
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
            entry->key = (void *)key;
        }
        ++map->size;
    }
    else if (entry->value)
    {
        /* Do not overwrite existing value */
        return entry->value;
    }

    entry->value = value;
    return value;
}

void* clib_hashmap_get(const clib_hashmap* map, const void* key)
{
    CLIB_ASSERT(map != NULL);
    CLIB_ASSERT(key != NULL);

    clib_hashmap_entry* entry = clib_hashmap_entry_find(map, key, false);

    if (!entry) return NULL;

    return entry->value;
}

void* clib_hashmap_remove(clib_hashmap* map, const void *key)
{
    CLIB_ASSERT(map != NULL);
    CLIB_ASSERT(key != NULL);

    clib_hashmap_entry* entry = clib_hashmap_entry_find(map, key, false);
    if (!entry)
        return NULL;

    void* value = entry->value;
    /* Clear the entry and make the chain contiguous */
    clib_hashmap_entry_remove(map, entry);
    return value;
}

void clib_hashmap_clear(clib_hashmap* map)
{
    CLIB_ASSERT(map != NULL);

    clib_hashmap_free_keys(map);
    map->size = 0;
    memset(map->table, 0, sizeof(clib_hashmap_entry) * map->capacity);
}

void clib_hashmap_reset(clib_hashmap* map)
{
    CLIB_ASSERT(map != NULL);

    clib_hashmap_clear(map);
    if (map->capacity == map->capacity_init)
        return;

    clib_hashmap_entry* new_table = (clib_hashmap_entry*)realloc(map->table, sizeof(clib_hashmap_entry) * map->capacity_init);
    if (!new_table)
        return;

    map->table = new_table;
    map->capacity = map->capacity_init;
}

clib_hashmap_iter* clib_hashmap_iterator(const clib_hashmap* map)
{
    CLIB_ASSERT(map != NULL);

    if (!map->size)
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

void clib_hashmap_iter_set_value(const clib_hashmap_iter* iter, void* value)
{
    if (!iter)
        return;

    ((clib_hashmap_entry*)iter)->value = value;
}

/*
 * Default hash function for string keys.
 * This is an implementation of the well-documented Jenkins one-at-a-time
 * hash function.
 */
size_t clib_hashmap_hash_string(const void* key)
{
    size_t hash = 0;

    for (const char* key_str = (const char*)key; *key_str; ++key_str)
    {
        hash += *key_str;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

/*
 * Default key comparator function for string keys.
 */
int clib_hashmap_compare_string(const void *a, const void *b)
{
    return strcmp((const char*)a, (const char*)b);
}

#ifdef CLIB_HASHMAP_METRICS
/*
 * Return the load factor.
 */
double clib_hashmap_load_factor(const clib_hashmap* map)
{
    CLIB_ASSERT(map != NULL);

    if (!map->table_size)
        return 0;

    return (double)map->num_entries / map->table_size;
}

/*
 * Return the average number of collisions per entry.
 */
double clib_hashmap_collisions_mean(const clib_hashmap *map)
{
    clib_hashmap_entry *entry;
    size_t total_collisions = 0;

    CLIB_ASSERT(map != NULL);

    if (!map->num_entries)
        return 0;

    for (entry = map->table; entry < &map->table[map->table_size]; ++entry) 
    {
        if (!entry->key)
            continue;

        total_collisions += entry->num_collisions;
    }

    return (double)total_collisions / map->num_entries;
}

/*
 * Return the variance between entry collisions.  The higher the variance,
 * the more likely the hash function is poor and is resulting in clustering.
 */
double clib_hashmap_collisions_variance(const clib_hashmap *map)
{
    clib_hashmap_entry *entry;
    double mean_collisions;
    double variance;
    double total_variance = 0;

    CLIB_ASSERT(map != NULL);

    if (!map->num_entries)
        return 0;

    mean_collisions = clib_hashmap_collisions_mean(map);
    for (entry = map->table; entry < &map->table[map->table_size]; ++entry) 
    {
        if (!entry->key)
            continue;

        variance = (double)entry->num_collisions - mean_collisions;
        total_variance += variance * variance;
    }
    return total_variance / map->num_entries;
}
#endif

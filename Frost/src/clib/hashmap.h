#ifndef CLIB_HASHMAP_H
#define CLIB_HASHMAP_H

#include <stddef.h>

typedef struct _clib_hashmap_iter clib_hashmap_iter;

#define CLIB_HASHMAP_ITERATE_FOR(dict, iter) \
    for (clib_hashmap_iter* iter = clib_hashmap_iterator(dict); iter; iter = clib_hashmap_iter_next(dict, iter))

typedef enum
{
    CLIB_HASHMAP_OK = 0,
    CLIB_HASHMAP_ALLOC_ERROR,
    CLIB_HASHMAP_HASH_ERROR,
    CLIB_HASHMAP_KEY_NOT_FOUND
} clib_hashmap_error;

typedef struct
{
    void* key;
    void* value;
} clib_hashmap_entry;

typedef struct
{
    clib_hashmap_entry* table;
    size_t capacity;
    size_t used;
    size_t  (*hash)(const void*);
    int     (*key_cmp)(const void*, const void*);
    /* memory */
    void*   (*key_alloc)(const void*);
    void    (*key_free)(void*);
    void*   (*value_alloc)(const void*);
    void    (*value_free)(void*);
} clib_hashmap;

/*
 * Initialize an empty hashmap.
 *
 * hash_func should return an even distribution of numbers between 0
 * and SIZE_MAX varying on the key provided.
 *
 * key_compare_func should return 0 if the keys match, and non-zero otherwise.
 *
 * initial_size is optional, and may be set to the max number of entries
 * expected to be put in the hash table.  This is used as a hint to
 * pre-allocate the hash table to the minimum size needed to avoid
 * gratuitous rehashes.  If initial_size is 0, a default size will be used.
 *
 * Returns CLIB_HASHMAP_OK on success and clib_hashmap_error on failure.
 */
clib_hashmap_error clib_hashmap_alloc(clib_hashmap* map, size_t (*hash_func)(const void*), int (*cmp_func)(const void*, const void*), size_t initial_capacity);

/*
 * Free the hashmap and all associated memory.
 */
void clib_hashmap_free(clib_hashmap* map);

/*
 * Enable internal memory allocation and management of keys and values.
 */
void clib_hashmap_set_key_alloc_funcs(clib_hashmap* map, void* (*alloc_func)(const void*), void (*free_func)(void*));
void clib_hashmap_set_value_alloc_funcs(clib_hashmap* map, void* (*alloc_func)(const void*), void (*free_func)(void*));

clib_hashmap_error clib_hashmap_rehash(clib_hashmap* map, size_t new_capacity);

/*
 * Insert an entry to the hashmap.  
 * If an entry with a matching key already exists and has a value pointer 
 * associated with it, the existing value pointer is returned, instead of 
 * assigning the new value.
 * Compare the return value with the value passed in to determine if a 
 * new entry was created.
 * Returns NULL if memory allocation failed.
 */
void* clib_hashmap_insert(clib_hashmap* map, const void* key, void* value);

/*
 * Remove an entry with the specified key from the map.
 * Returns CLIB_HASHMAP_KEY_NOT_FOUND if no entry was found, else CLIB_HASHMAP_OK.
 */
clib_hashmap_error clib_hashmap_remove(clib_hashmap* map, const void *key);

/*
 * Remove all entries.
 */
void clib_hashmap_clear(clib_hashmap* map);

/*
 * Return the value pointer, or NULL if no entry was found.
 */
void* clib_hashmap_find(const clib_hashmap* map, const void* key);

/*
 * Find the hashmap entry with the specified key, or an empty slot.
 * Returns NULL if the entire table has been searched without finding a match.
 */
clib_hashmap_entry* clib_hashmap_entry_find(const clib_hashmap* map, const void* key, int find_empty);

/*
 * Removes the specified entry and processes the proceeding entries to reduce
 * the load factor and keep the chain continuous.
 * This is a required step for hash maps using linear probing.
 */
void clib_hashmap_entry_remove(clib_hashmap* map, clib_hashmap_entry* removed_entry);

/*
 * Get a new hashmap iterator.
 * The iterator is an opaque pointer that may be used with hashmap_iter_*() 
 * functions.
 * Hashmap iterators are INVALID after a remove operation is performed.
 * hashmap_iter_remove() allows safe removal during iteration.
 */
clib_hashmap_iter* clib_hashmap_iterator(const clib_hashmap* map);

/*
 * Return an iterator to the next hashmap entry.
 * Returns NULL if there are no more entries.
 */
clib_hashmap_iter* clib_hashmap_iter_next(const clib_hashmap* map, const clib_hashmap_iter* iter);

/*
 * Remove the hashmap entry pointed to by this iterator and returns an
 * iterator to the next entry.
 * Returns NULL if there are no more entries.
 */
clib_hashmap_iter* clib_hashmap_iter_remove(clib_hashmap* map, const clib_hashmap_iter* iter);

/*
 * Return the key of the entry pointed to by the iterator.
 */
const void* clib_hashmap_iter_get_key(const clib_hashmap_iter* iter);

/*
 * Return the value of the entry pointed to by the iterator.
 */
void* clib_hashmap_iter_get_value(const clib_hashmap_iter* iter);

#endif /* !CLIB_HASHMAP_H */

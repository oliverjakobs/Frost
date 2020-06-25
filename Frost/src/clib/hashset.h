#ifndef CLIB_HASHSET_H
#define CLIB_HASHSET_H

#include <stddef.h>
#include <stdint.h>

typedef struct _clib_hashset_iter clib_hashset_iter;

#define CLIB_HASHSET_ITERATE_FOR(set, iter) \
    for (clib_hashset_iter* iter = clib_hashset_iterator(set); iter; iter = clib_hashset_iter_next(set, iter))

typedef enum
{
    CLIB_HASHSET_OK = 0,
    CLIB_HASHSET_ALLOC_ERROR,
    CLIB_HASHSET_HASH_ERROR,
    CLIB_HASHSET_KEY_NOT_FOUND
} clib_hashset_error;

typedef struct
{
    int32_t key;
    void* value;
} clib_hashset_entry;

typedef struct
{
    clib_hashset_entry* table;
    size_t capacity;
    size_t used;
    size_t  (*hash)(int32_t);
    /* memory */
    void*   (*value_alloc)(const void*);
    void    (*value_free)(void*);
} clib_hashset;

/*
 * Initialize an empty hashset.
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
 * Returns CLIB_HASHSET_OK on success and clib_hashset_error on failure.
 */
clib_hashset_error clib_hashset_alloc(clib_hashset* set, size_t(*hash_func)(int32_t), size_t initial_capacity);

/*
 * Free the hashset and all associated memory.
 */
void clib_hashset_free(clib_hashset* set);

/*
 * Enable internal memory allocation and management of values.
 */
void clib_hashset_set_value_alloc_funcs(clib_hashset* set, void* (*alloc_func)(const void*), void (*free_func)(void*));

clib_hashset_error clib_hashset_rehash(clib_hashset* set, size_t new_capacity);

/*
 * Insert an entry to the hashset.
 * If an entry with a matching key already exists and has a value pointer
 * associated with it, the existing value pointer is returned, instead of
 * assigning the new value.
 * Compare the return value with the value passed in to determine if a
 * new entry was created.
 * Returns NULL if memory allocation failed.
 */
void* clib_hashset_insert(clib_hashset* set, int32_t key, void* value);

/*
 * Remove an entry with the specified key from the set.
 * Returns CLIB_HASHset_KEY_NOT_FOUND if no entry was found, else CLIB_HASHSET_OK.
 */
clib_hashset_error clib_hashset_remove(clib_hashset* set, int32_t key);

/*
 * Remove all entries.
 */
void clib_hashset_clear(clib_hashset* set);

/*
 * Return the value pointer, or NULL if no entry was found.
 */
void* clib_hashset_find(const clib_hashset* set, int32_t key);

/*
 * Find the hashset entry with the specified key, or an empty slot.
 * Returns NULL if the entire table has been searched without finding a match.
 */
clib_hashset_entry* clib_hashset_entry_find(const clib_hashset* set, int32_t key, int find_empty);

/*
 * Removes the specified entry and processes the proceeding entries to reduce
 * the load factor and keep the chain continuous.
 * This is a required step for hash sets using linear probing.
 */
void clib_hashset_entry_remove(clib_hashset* set, clib_hashset_entry* removed_entry);

/*
 * Get a new hashset iterator.
 * The iterator is an opaque pointer that may be used with hashset_iter_*()
 * functions.
 * Hashset iterators are INVALID after a remove operation is performed.
 * hashset_iter_remove() allows safe removal during iteration.
 */
clib_hashset_iter* clib_hashset_iterator(const clib_hashset* set);

/*
 * Return an iterator to the next hashset entry.
 * Returns NULL if there are no more entries.
 */
clib_hashset_iter* clib_hashset_iter_next(const clib_hashset* set, const clib_hashset_iter* iter);

/*
 * Remove the hashset entry pointed to by this iterator and returns an
 * iterator to the next entry.
 * Returns NULL if there are no more entries.
 */
clib_hashset_iter* clib_hashset_iter_remove(clib_hashset* set, const clib_hashset_iter* iter);

/*
 * Return the key of the entry pointed to by the iterator.
 */
int32_t clib_hashset_iter_get_key(const clib_hashset_iter* iter);

/*
 * Return the value of the entry pointed to by the iterator.
 */
void* clib_hashset_iter_get_value(const clib_hashset_iter* iter);

#endif /* !CLIB_HASHSET_H */
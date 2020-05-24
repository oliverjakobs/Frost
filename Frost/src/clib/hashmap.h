/*
 * Copyright (c) 2016-2018 David Leeds <davidesleeds@gmail.com>
 *
 * Hashmap is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#ifndef CLIB_HASHMAP_H
#define CLIB_HASHMAP_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

/*
 * Define CLIB_HASHMAP_METRICS to compile in performance analysis
 * functions for use in assessing hash function performance.
 */
/* #define CLIB_HASHMAP_METRICS */

/*
 * Macros to declare type-specific versions of hashmap_*() functions to
 * allow compile-time type checking and avoid the need for type casting.
 */
#define CLIB_HASHMAP_DECLARE_FUNCS(name, key_type, value_type)                              \
    value_type* name##_hashmap_insert(clib_hashmap* map, const key_type* key, value_type* value);   \
    value_type* name##_hashmap_get(const clib_hashmap* map, const key_type* key);                \
    value_type* name##_hashmap_remove(clib_hashmap* map, const key_type* key);                   \
    const key_type* name##_hashmap_iter_get_key(const clib_hashmap_iter* iter);           \
    value_type* name##_hashmap_iter_get_value(const clib_hashmap_iter* iter);             \
    void name##_hashmap_iter_set_value(const clib_hashmap_iter* iter, value_type* value);


#define CLIB_HASHMAP_DEFINE_FUNCS(name, key_type, value_type)                               \
    value_type* name##_hashmap_insert(clib_hashmap* map, const key_type* key, value_type* value)    \
    {                                                                                       \
        return (value_type*)clib_hashmap_insert(map, (const void*)key, (void*)value);               \
    }                                                                                       \
    value_type* name##_hashmap_get(const clib_hashmap* map, const key_type* key)                 \
    {                                                                                       \
        return (value_type*)clib_hashmap_get(map, (const void*)key);                             \
    }                                                                                       \
    value_type* name##_hashmap_remove(clib_hashmap* map, const key_type* key)                    \
    {                                                                                       \
        return (value_type*)clib_hashmap_remove(map, (const void*)key);                          \
    }                                                                                       \
    const key_type* name##_hashmap_iter_get_key(const clib_hashmap_iter* iter)            \
    {                                                                                       \
        return (const key_type*)clib_hashmap_iter_get_key(iter);                                 \
    }                                                                                       \
    value_type* name##_hashmap_iter_get_value(const clib_hashmap_iter* iter)              \
    {                                                                                       \
        return (value_type*)clib_hashmap_iter_get_value(iter);                                   \
    }                                                                                       \
    void name##_hashmap_iter_set_value(const clib_hashmap_iter* iter, value_type* value)  \
    {                                                                                       \
        clib_hashmap_iter_set_value(iter, (void*)value);                                         \
    }

typedef struct
{
    void* key;
    void* value;

#ifdef CLIB_HASHMAP_METRICS
    size_t num_collisions;
#endif
} clib_hashmap_entry;

typedef struct clib_hashmap_iter_s clib_hashmap_iter;

/*
 * The hashmap state structure.
 */
typedef struct clib_hashmap_s
{
    size_t capacity_init;
    size_t capacity;
    size_t size;
    clib_hashmap_entry* table;
    size_t  (*hash)(const void*);
    int     (*key_compare)(const void*, const void*);
    void*   (*key_alloc)(const void*);
    void    (*key_free)(void*);
} clib_hashmap;

/*
 * Initialize an empty hashmap.
 *
 * hash_func should return an even distribution of numbers between 0
 * and SIZE_MAX varying on the key provided.  If set to NULL, the default
 * case-sensitive string hash function is used: hashmap_hash_string
 *
 * key_compare_func should return 0 if the keys match, and non-zero otherwise.
 * If set to NULL, the default case-sensitive string comparator function is
 * used: hashmap_compare_string
 *
 * initial_size is optional, and may be set to the max number of entries
 * expected to be put in the hash table.  This is used as a hint to
 * pre-allocate the hash table to the minimum size needed to avoid
 * gratuitous rehashes.  If initial_size is 0, a default size will be used.
 *
 * Returns 0 on success and -errno on failure.
 */
int clib_hashmap_init(clib_hashmap* map, size_t (*hash_func)(const void*), int (*comp_func)(const void*, const void*), size_t initial_size);

/*
 * Free the hashmap and all associated memory.
 */
void clib_hashmap_destroy(clib_hashmap* map);

/*
 * Enable internal memory allocation and management of hash keys.
 */
void clib_hashmap_set_key_alloc_funcs(clib_hashmap* map, void* (*key_alloc_func)(const void*), void (*key_free_func)(void*));

/*
 * Add an entry to the hashmap.  If an entry with a matching key already
 * exists and has a value pointer associated with it, the existing value
 * pointer is returned, instead of assigning the new value.  
 * Compare the return value with the value passed in to determine if a 
 * new entry was created.
 * Returns NULL if memory allocation failed.
 */
void* clib_hashmap_insert(clib_hashmap* map, const void* key, void* value);

/*
 * Return the value pointer, or NULL if no entry exists.
 */
void* clib_hashmap_get(const clib_hashmap* map, const void* key);

/*
 * Remove an entry with the specified key from the map.
 * Returns the value pointer, or NULL, if no entry was found.
 */
void* clib_hashmap_remove(clib_hashmap* map, const void* key);

/*
 * Remove all entries.
 */
void clib_hashmap_clear(clib_hashmap* map);

/*
 * Remove all entries and reset the hash table to its initial size.
 */
void clib_hashmap_reset(clib_hashmap* map);

/*
 * Get a new hashmap iterator.  The iterator is an opaque
 * pointer that may be used with hashmap_iter_*() functions.
 * Hashmap iterators are INVALID after a put or remove operation is performed.
 * hashmap_iter_remove() allows safe removal during iteration.
 */
clib_hashmap_iter* clib_hashmap_iterator(const clib_hashmap* map);

/*
 * Return an iterator to the next hashmap entry.  Returns NULL if there are
 * no more entries.
 */
clib_hashmap_iter* clib_hashmap_iter_next(const clib_hashmap* map, const clib_hashmap_iter* iter);

/*
 * Remove the hashmap entry pointed to by this iterator and returns an
 * iterator to the next entry.  Returns NULL if there are no more entries.
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

/*
 * Set the value pointer of the entry pointed to by the iterator.
 */
void clib_hashmap_iter_set_value(const clib_hashmap_iter* iter, void* value);

/*
 * Default hash function for string keys.
 * This is an implementation of the well-documented Jenkins one-at-a-time
 * hash function.
 */
size_t clib_hashmap_hash_string(const void* key);

/*
 * Default key comparator function for string keys.
 */
int clib_hashmap_compare_string(const void* a, const void* b);


#ifdef CLIB_HASHMAP_METRICS
/*
 * Return the load factor.
 */
double clib_hashmap_load_factor(const clib_hashmap* map);

/*
 * Return the average number of collisions per entry.
 */
double clib_hashmap_collisions_mean(const clib_hashmap* map);

/*
 * Return the variance between entry collisions.  The higher the variance,
 * the more likely the hash function is poor and is resulting in clustering.
 */
double clib_hashmap_collisions_variance(const clib_hashmap* map);
#endif

#endif /* !CLIB_HASHMAP_H */
#ifndef CLIB_FOREACH_H
#define CLIB_FOREACH_H

#include "hashmap.h"

/*
 * Macros to declare type-specific versions of hashmap_foreach_*() functions to
 * allow compile-time type checking and avoid the need for type casting.
 */
#define CLIB_HASHMAP_DECLARE_FUNCS_FOR_EACH(name, key_type, value_type)         \
    int name##_hashmap_foreach(const clib_hashmap* map, int (*func)(const key_type*, value_type*, void*), void* arg);

#define CLIB_HASHMAP_DEFINE_FUNCS_FOR_EACH(name, key_type, value_type)          \
    struct __##name##_hashmap_foreach_state                                     \
    {                                                                           \
        int (*func)(const key_type*, value_type*, void*);                       \
        void* arg;                                                              \
    };                                                                          \
    static inline int __##name##_hashmap_foreach_callback(const void* key, void* value, void* arg) \
    {                                                                           \
        struct __##name##_hashmap_foreach_state* s = (struct __##name##_hashmap_foreach_state*)arg; \
        return s->func((const key_type*)key, (value_type*)value, s->arg);       \
    }                                                                           \
    int name##_hashmap_foreach(const clib_hashmap* map, int (*func)(const key_type*, value_type*, void*), void* arg) \
    {                                                                           \
        struct __##name##_hashmap_foreach_state s = { func, arg };              \
        return clib_hashmap_foreach(map, __##name##_hashmap_foreach_callback, &s);   \
    }

 /*
  * Invoke func for each entry in the hashmap.  Unlike the hashmap_iter_*()
  * interface, this function supports calls to hashmap_remove() during iteration.
  * However, it is an error to put or remove an entry other than the current one,
  * and doing so will immediately halt iteration and return an error.
  * Iteration is stopped if func returns non-zero.  
  * Returns func's return value if it is < 0, otherwise, 0.
  */
int clib_hashmap_foreach(const clib_hashmap* map, int (*func)(const void*, void*, void*), void* arg);

#endif /* !CLIB_FOREACH_H */

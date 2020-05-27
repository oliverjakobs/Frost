#ifndef CLIB_DICTIONARY_H
#define CLIB_DICTIONARY_H

#include "hashmap.h"

#define CLIB_DICT_DECLARE_FUNCS(name, type)                                     \
    type* name##_dict_insert(clib_dict* dict, const char* key, type* value);    \
    type* name##_dict_get(const clib_dict* dict, const char* key);              \
    type* name##_dict_remove(clib_dict* dict, const char* key);                 \
    type* name##_dict_iter_get_value(const clib_dict_iter* iter);               \
    void name##_dict_iter_set_value(const clib_dict_iter* iter, type* value);


#define CLIB_DICT_DEFINE_FUNCS(name, type)                                  \
    type* name##_dict_insert(clib_dict* dict, const char* key, type* value) \
    {                                                                       \
        return (type*)clib_dict_insert(dict, key, (void*)value);            \
    }                                                                       \
    type* name##_dict_get(const clib_dict* dict, const char* key)           \
    {                                                                       \
        return (type*)clib_dict_get(dict, key);                             \
    }                                                                       \
    type* name##_dict_remove(clib_dict* dict, const char* key)              \
    {                                                                       \
        return (type*)clib_dict_remove(dict, key);                          \
    }                                                                       \
    type* name##_dict_iter_get_value(const clib_dict_iter* iter)            \
    {                                                                       \
        return (type*)clib_dict_iter_get_value(iter);                       \
    }                                                                       \
    void name##_dict_iter_set_value(const clib_dict_iter* iter, type* value)\
    {                                                                       \
        clib_dict_iter_set_value(iter, (void*)value);                       \
    }

#define CLIB_DICT_ITERATE_FOR(dict) for (clib_dict_iter* iter = clib_dict_iterator(dict); iter; iter = clib_dict_iter_next(dict, iter))

typedef struct clib_hashmap_s clib_dict;
typedef struct clib_hashmap_iter_s clib_dict_iter;

int clib_dict_init(clib_dict* dict, size_t initial_size);
void clib_dict_destroy(clib_dict* dict);

void* clib_dict_insert(clib_dict* dict, const char* key, void* value);
void* clib_dict_get(const clib_dict* dict, const char* key);
void* clib_dict_remove(clib_dict* dict, const char* key);

void clib_dict_clear(clib_dict* dict);
void clib_dict_reset(clib_dict* dict);

clib_dict_iter* clib_dict_iterator(const clib_dict* dict);
clib_dict_iter* clib_dict_iter_next(const clib_dict* dict, const clib_dict_iter* iter);
clib_dict_iter* clib_dict_iter_remove(clib_dict* dict, const clib_dict_iter* iter);

const char* clib_dict_iter_get_key(const clib_dict_iter* iter);
void* clib_dict_iter_get_value(const clib_dict_iter* iter);
void clib_dict_iter_set_value(const clib_dict_iter* iter, void* value);

#endif /* !CLIB_DICTIONARY_H */

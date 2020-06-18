#ifndef CLIB_DICTIONARY_H
#define CLIB_DICTIONARY_H

#include "hashmap.h"

#define CLIB_DICT_ITERATE_FOR(dict, iter) for (clib_hashmap_iter* iter = clib_dict_iterator(dict); iter; iter = clib_dict_iter_next(dict, iter))

clib_hashmap_error clib_dict_alloc(clib_hashmap* dict, size_t initial_size);
void clib_dict_free(clib_hashmap* dict);

void* clib_dict_insert(clib_hashmap* dict, const char* key, void* value);
void* clib_dict_find(const clib_hashmap* dict, const char* key);
const char* clib_dict_get_key_ptr(const clib_hashmap * dict, const char* key);
clib_hashmap_error clib_dict_remove(clib_hashmap* dict, const char* key);

void clib_dict_clear(clib_hashmap* dict);

clib_hashmap_iter* clib_dict_iterator(const clib_hashmap* dict);
clib_hashmap_iter* clib_dict_iter_next(const clib_hashmap* dict, const clib_hashmap_iter* iter);
clib_hashmap_iter* clib_dict_iter_remove(clib_hashmap* dict, const clib_hashmap_iter* iter);

const char* clib_dict_iter_get_key(const clib_hashmap_iter* iter);
void* clib_dict_iter_get_value(const clib_hashmap_iter* iter);

#endif /* !CLIB_DICTIONARY_H */

#ifndef CLIB_STRMAP_H
#define CLIB_STRMAP_H

#include "dictionary.h"

#define CLIB_STRMAP_ITERATE_FOR(map) for (clib_hashmap_iter* iter = clib_strmap_iterator(map); iter; iter = clib_strmap_iter_next(map, iter))

clib_hashmap_error clib_strmap_alloc(clib_hashmap* map, size_t initial_size);
void clib_strmap_free(clib_hashmap* map);

char* clib_strmap_insert(clib_hashmap* map, const char* key, char* value);
char* clib_strmap_find(const clib_hashmap* map, const char* key);
clib_hashmap_error clib_strmap_remove(clib_hashmap* map, const char* key);

void clib_strmap_clear(clib_hashmap* map);

clib_hashmap_iter* clib_strmap_iterator(const clib_hashmap* map);
clib_hashmap_iter* clib_strmap_iter_next(const clib_hashmap* map, const clib_hashmap_iter* iter);
clib_hashmap_iter* clib_strmap_iter_remove(clib_hashmap* map, const clib_hashmap_iter* iter);

const char* clib_strmap_iter_get_key(const clib_hashmap_iter* iter);
char* clib_strmap_iter_get_value(const clib_hashmap_iter* iter);

#endif /* !CLIB_STRMAP_H */
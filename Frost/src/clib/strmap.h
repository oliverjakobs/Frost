#ifndef CLIB_STRMAP_H
#define CLIB_STRMAP_H

#include "hashmap.h"

typedef struct clib_hashmap_iter_s clib_strmap_iter;

int clib_strmap_init(clib_hashmap* map, size_t initial_size);
void clib_strmap_destroy(clib_hashmap* map);

char* clib_strmap_insert(clib_hashmap* map, const char* key, char* value);
char* clib_strmap_get(const clib_hashmap* map, const char* key);
char* clib_strmap_remove(clib_hashmap* map, const char* key);

const char* clib_strmap_iter_get_key(const clib_strmap_iter* iter);
char* clib_strmap_iter_get_value(const clib_strmap_iter* iter);
void clib_strmap_iter_set_value(const clib_strmap_iter* iter, char* value);

#endif /* !CLIB_STRMAP_H */
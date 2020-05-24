#ifndef CLIB_STRMAP_H
#define CLIB_STRMAP_H

#include "dictionary.h"

typedef struct clib_hashmap_s clib_strmap;
typedef struct clib_hashmap_iter_s clib_strmap_iter;

int clib_strmap_init(clib_strmap* map, size_t initial_size);
void clib_strmap_destroy(clib_strmap* map);

char* clib_strmap_insert(clib_strmap* map, const char* key, char* value);
char* clib_strmap_get(const clib_strmap* map, const char* key);
char* clib_strmap_remove(clib_strmap* map, const char* key);

void clib_strmap_clear(clib_strmap* map);
void clib_strmap_reset(clib_strmap* map);

clib_strmap_iter* clib_strmap_iterator(const clib_strmap* map);
clib_strmap_iter* clib_strmap_iter_next(const clib_strmap* map, const clib_strmap_iter* iter);
clib_strmap_iter* clib_strmap_iter_remove(clib_strmap* map, const clib_strmap_iter* iter);

const char* clib_strmap_iter_get_key(const clib_strmap_iter* iter);
char* clib_strmap_iter_get_value(const clib_strmap_iter* iter);
void clib_strmap_iter_set_value(const clib_strmap_iter* iter, char* value);

#endif /* !CLIB_STRMAP_H */
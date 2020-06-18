#include "strmap.h"

#include <string.h>
#include <stdlib.h>

clib_hashmap_error clib_strmap_alloc(clib_hashmap* map, size_t initial_size)
{
    return clib_dict_alloc(map, initial_size);
}

void clib_strmap_free(clib_hashmap* map)
{
    clib_hashmap_free(map);
}

char* clib_strmap_insert(clib_hashmap* map, const char* key, char* val)
{
    size_t len = strlen(val);
    char* value = (char*)malloc(len);

    if (!value) return NULL;

    strcpy(value, val);

    if ((char*)clib_hashmap_insert(map, (const void*)key, (void*)value) != value)
    {
        free(value);
        return NULL;
    }

    return value;
}

char* clib_strmap_find(const clib_hashmap* map, const char* key)
{
    return (char*)clib_hashmap_find(map, (const void*)key);
}

clib_hashmap_error clib_strmap_remove(clib_hashmap* map, const char* key)
{
    return clib_hashmap_remove(map, (const void*)key);
}

void clib_strmap_clear(clib_hashmap* map)
{
    clib_hashmap_clear(map);
}

clib_hashmap_iter* clib_strmap_iterator(const clib_hashmap* map)
{
    return clib_hashmap_iterator(map);
}

clib_hashmap_iter* clib_strmap_iter_next(const clib_hashmap* map, const clib_hashmap_iter* iter)
{
    return clib_hashmap_iter_next(map, iter);
}

clib_hashmap_iter* clib_strmap_iter_remove(clib_hashmap* map, const clib_hashmap_iter* iter)
{
    return clib_hashmap_iter_remove(map, iter);
}

const char* clib_strmap_iter_get_key(const clib_hashmap_iter* iter)
{
    return (const char*)clib_hashmap_iter_get_key(iter);
}

char* clib_strmap_iter_get_value(const clib_hashmap_iter* iter)
{
    return (char*)clib_hashmap_iter_get_value(iter);
}
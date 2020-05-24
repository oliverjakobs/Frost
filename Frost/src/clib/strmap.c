#include "strmap.h"



int clib_strmap_init(clib_strmap* map, size_t initial_size)
{
    return clib_dict_init(map, initial_size);
}

void clib_strmap_destroy(clib_strmap* map)
{
    clib_hashmap_destroy(map);
}

char* clib_strmap_insert(clib_strmap* map, const char* key, char* val)
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

char* clib_strmap_get(const clib_strmap* map, const char* key)
{
    return (char*)clib_hashmap_get(map, (const void*)key);
}

char* clib_strmap_remove(clib_strmap* map, const char* key)
{
    return (char*)clib_hashmap_remove(map, (const void*)key);
}

void clib_strmap_clear(clib_strmap* map)
{
    clib_hashmap_clear(map);
}

void clib_strmap_reset(clib_strmap* map)
{
    clib_hashmap_reset(map);
}

clib_strmap_iter* clib_strmap_iterator(const clib_strmap* map)
{
    return clib_hashmap_iterator(map);
}

clib_strmap_iter* clib_strmap_iter_next(const clib_strmap* map, const clib_strmap_iter* iter)
{
    return clib_hashmap_iter_next(map, iter);
}

clib_strmap_iter* clib_strmap_iter_remove(clib_strmap* map, const clib_strmap_iter* iter)
{
    return clib_hashmap_iter_remove(map, iter);
}

const char* clib_strmap_iter_get_key(const clib_strmap_iter* iter)
{
    return (const char*)clib_hashmap_iter_get_key(iter);
}

char* clib_strmap_iter_get_value(const clib_strmap_iter* iter)
{
    return (char*)clib_hashmap_iter_get_value(iter);
}

void clib_strmap_iter_set_value(const clib_strmap_iter* iter, char* value)
{
    // clib_hashmap_iter_set_value(iter, (void*)value);
}
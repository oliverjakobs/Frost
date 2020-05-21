#include "strmap.h"

static void* _clib_strmap_key_alloc(const void* src)
{
    size_t size = sizeof(src);
    void* val = malloc(size);

    if (!val) return NULL;

    memcpy(val, src, size);

    return val;
}

static void _clib_strmap_key_free(void* key)
{
    free(key);
}

int clib_strmap_init(clib_hashmap* map, size_t initial_size)
{
    int err = clib_hashmap_init(map, clib_hashmap_hash_string, clib_hashmap_compare_string, initial_size);

    if (!err)
    {
        clib_hashmap_set_key_alloc_funcs(map, _clib_strmap_key_alloc, _clib_strmap_key_free);
    }

    return err;
}

void clib_strmap_destroy(clib_hashmap* map)
{
    clib_hashmap_destroy(map);
}

char* clib_strmap_put(clib_hashmap* map, const char* key, char* val)
{
    char* value = (char*)malloc(strlen(val));

    if (!value) return NULL;

    strcpy(value, val);

    if ((char*)clib_hashmap_put(map, (const void*)key, (void*)value) != value)
    {
        free(value);
        return NULL;
    }

    return value;
}

char* clib_strmap_get(const clib_hashmap* map, const char* key)
{
    return (char*)clib_hashmap_get(map, (const void*)key);
}

char* clib_strmap_remove(clib_hashmap* map, const char* key)
{
    return (char*)clib_hashmap_remove(map, (const void*)key);
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
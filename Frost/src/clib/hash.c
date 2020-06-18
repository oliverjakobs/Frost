#include "hash.h"

size_t clib_hash_string(const char* str)
{
    size_t hash = 0;

    for (const char* str_hash = str; *str_hash; ++str_hash)
    {
        hash += *str_hash;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}
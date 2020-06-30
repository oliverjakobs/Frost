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

uint32_t clib_hash_uint32(uint32_t i)
{
    uint32_t hash = i;
    hash = ((hash >> 16) ^ hash) * 0x45d9f3b;
    hash = ((hash >> 16) ^ hash) * 0x45d9f3b;
    hash = (hash >> 16) ^ hash;
    return hash;
}

uint64_t clib_hash_uint64(uint64_t i)
{
    uint64_t hash = i;
    hash = (hash ^ (hash >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    hash = (hash ^ (hash >> 27)) * UINT64_C(0x94d049bb133111eb);
    hash = hash ^ (hash >> 31);
    return hash;
}

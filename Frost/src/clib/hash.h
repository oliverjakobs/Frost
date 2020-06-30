#ifndef CLIB_HASH_H
#define CLIB_HASH_H

#include <stddef.h>
#include <stdint.h>

size_t clib_hash_string(const char* str);

uint32_t clib_hash_uint32(uint32_t i);
uint64_t clib_hash_uint64(uint64_t i);

#endif /* !CLIB_HASH_H */
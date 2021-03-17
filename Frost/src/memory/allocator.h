#ifndef TB_ALLOCATOR_H
#define TB_ALLOCATOR_H

#include <stdlib.h>

typedef struct
{
    void* (*alloc)(void*, size_t);
    void* (*realloc)(void*, void*, size_t);
    void (*free)(void*, void*, size_t);
    void* data;
} tb_allocator;

void* tb_mem_alloc(tb_allocator* allocator, size_t size);
void* tb_mem_realloc(tb_allocator* allocator, void* block, size_t size);
void tb_mem_free(tb_allocator* allocator, void* block, size_t size);

#endif /* !TB_ALLOCATOR_H */

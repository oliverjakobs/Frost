#ifndef TB_ALLOCATOR_H
#define TB_ALLOCATOR_H

#include <stdlib.h>

#define TB_MEM_HDR(block) ((size_t*)(void*)(block) - 1)

typedef struct
{
    void* (*alloc)(void*, size_t);
    void (*free)(void*, void*, size_t);
    void* data;
} tb_allocator;

void* tb_mem_alloc(tb_allocator* allocator, size_t size);
void tb_mem_free(tb_allocator* allocator, void* block);

void* tb_mem_dup(tb_allocator* allocator, const void* src, size_t size);

#endif /* !TB_ALLOCATOR_H */

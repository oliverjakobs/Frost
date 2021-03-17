#include "allocator.h"


void* tb_mem_alloc(tb_allocator* allocator, size_t size)
{
    if (allocator && allocator->alloc) return allocator->alloc(allocator->data, size);
    return malloc(size);
}

void* tb_mem_realloc(tb_allocator* allocator, void* block, size_t size)
{
    if (allocator && allocator->realloc) return allocator->realloc(allocator->data, block, size);
    return realloc(block, size);
}

void tb_mem_free(tb_allocator* allocator, void* block, size_t size)
{
    if (allocator && allocator->free)
    {
        allocator->free(allocator->data, block, size);
        return;
    }

    free(block);
}

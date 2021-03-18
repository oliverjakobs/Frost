#include "allocator.h"

#include <string.h>

void* tb_mem_alloc(tb_allocator* allocator, size_t size)
{
    size += sizeof(size_t);
    size_t* hdr = (allocator && allocator->alloc) ? allocator->alloc(allocator->data, size) : malloc(size);

    if (!hdr) return NULL;

    hdr[0] = size;

    return hdr + 1;
}

void tb_mem_free(tb_allocator* allocator, void* block)
{
    if (!block) return;

    size_t* hdr = TB_MEM_HDR(block);
    if (allocator && allocator->free)
        allocator->free(allocator->data, hdr, hdr[0]);
    else
        free(hdr);
}

void* tb_mem_dup(tb_allocator* allocator, const void* src, size_t size)
{
    void* dst = tb_mem_alloc(allocator, size);
    
    return dst ? memcpy(dst, src, size) : NULL;
}

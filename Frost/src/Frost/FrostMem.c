#include "FrostMem.h"

#include "Application/Logger.h"

typedef struct
{
    size_t allocated;
} FrostMemTrace;

static FrostMemTrace frost_mem_trace = { 0 };

static void* FrostAlloc(size_t size)
{
    void* block = malloc(size);
    if (block) frost_mem_trace.allocated += size;
    return block;
}

static void FrostFree(void* block, size_t size)
{
    if (size > frost_mem_trace.allocated)
        DEBUG_WARN("Freeing %llu unallocated bytes", size - frost_mem_trace.allocated);

    free(block);
    frost_mem_trace.allocated -= size;
}

static tb_allocator frost_allocator = 
{
    .alloc = FrostAlloc,
    .realloc = NULL,
    .free = FrostFree,
};

tb_allocator* FrostGetAllocator()
{
    return &frost_allocator;
}

size_t FrostGetMemBytes()
{
    return frost_mem_trace.allocated;
}

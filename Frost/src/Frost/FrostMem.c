#include "FrostMem.h"

#include "Application/Logger.h"

#include "toolbox/toolbox.h"

typedef struct
{
    size_t allocated;
    size_t peak;
} FrostMemTrace;

static FrostMemTrace frost_mem_trace = { 0 };

static void* FrostAllocatorAlloc(size_t size)
{
    void* block = malloc(size);
    if (block)
    {
        frost_mem_trace.allocated += size;
        frost_mem_trace.peak = tb_max64(frost_mem_trace.peak, frost_mem_trace.allocated);
    }
    return block;
}

static void FrostAllocatorFree(void* block, size_t size)
{
    if (size > frost_mem_trace.allocated)
        DEBUG_WARN("Freeing %llu unallocated bytes", size - frost_mem_trace.allocated);

    free(block);
    frost_mem_trace.allocated -= size;
}

static tb_allocator frost_allocator = 
{
    .alloc = FrostAllocatorAlloc,
    .realloc = NULL,
    .free = FrostAllocatorFree,
};

void* FrostMalloc(size_t size)  { return tb_mem_alloc(&frost_allocator, size); }
void  FrostFree(void* block)    { tb_mem_free(&frost_allocator, block); }

tb_allocator*   FrostGetAllocator() { return &frost_allocator; }
size_t          FrostMemGetBytes()  { return frost_mem_trace.allocated; }
size_t          FrostMemGetPeak()   { return frost_mem_trace.peak; }

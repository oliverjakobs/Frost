#include "Memory.h"

#include "Minimal/Minimal.h"

#include "toolbox/toolbox.h"

typedef struct
{
    size_t allocated;
    size_t peak;
} FrostMemoryTrace;

static FrostMemoryTrace _memory_trace = { 0 };

static void* FrostAllocatorAlloc(size_t size)
{
    void* block = malloc(size);
    if (block)
    {
        _memory_trace.allocated += size;
        _memory_trace.peak = tb_max64(_memory_trace.peak, _memory_trace.allocated);
    }
    return block;
}

static void* FrostAllocatorRealloc(void* block, size_t old_size, size_t new_size)
{
    void* new_block = realloc(block, new_size);
    if (new_block)
    {
        _memory_trace.allocated += (new_size - old_size);
        _memory_trace.peak = tb_max64(_memory_trace.peak, _memory_trace.allocated);
    }
    return new_block;
}

static void FrostAllocatorFree(void* block, size_t size)
{
    if (size > _memory_trace.allocated)
        MINIMAL_WARN("Freeing %llu untraced bytes", size - _memory_trace.allocated);

    free(block);
    _memory_trace.allocated -= size;
}

static tb_allocator debug_allocator =
{
    .malloc = FrostAllocatorAlloc,
    .realloc = FrostAllocatorRealloc,
    .free = FrostAllocatorFree,
};

void* FrostMalloc(size_t size)                  { return tb_mem_malloc(&debug_allocator, size); }
void* FrostRealloc(void* block, size_t size)    { return tb_mem_realloc(&debug_allocator, block, size); }
void* FrostMemDup(const void* src, size_t size) { return tb_mem_dup(&debug_allocator, src, size); }
void  FrostFree(void* block)                    { tb_mem_free(&debug_allocator, block); }
void  FrostConstFree(const void* block)         { tb_mem_constfree(&debug_allocator, block); }

void* FrostArrayAlloc(void* block, size_t size)
{
    if (!block)
    {
        MINIMAL_INFO("Array Alloc: %d", size);
    } 
    else if (size == 0)
    {
        MINIMAL_INFO("Array Free: %d", size);
    }
    else
    {
        MINIMAL_INFO("Array Realloc: %d", size);
    }

    return realloc(block, size);
}

tb_allocator* FrostMemoryGetAllocator() { return &debug_allocator; }
size_t        FrostMemoryGetBytes()     { return _memory_trace.allocated; }
size_t        FrostMemoryGetPeak()      { return _memory_trace.peak; }

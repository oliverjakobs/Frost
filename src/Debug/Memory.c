#include "Memory.h"

#include "Minimal/Minimal.h"

#include "toolbox/toolbox.h"

typedef struct
{
    size_t allocated;
    size_t peak;
} DebugMemoryTrace;

static DebugMemoryTrace _memory_trace = { 0 };

static void* DebugAllocatorAlloc(size_t size)
{
    void* block = malloc(size);
    if (block)
    {
        _memory_trace.allocated += size;
        _memory_trace.peak = tb_max64(_memory_trace.peak, _memory_trace.allocated);
    }
    return block;
}

static void* DebugAllocatorRealloc(void* block, size_t old_size, size_t new_size)
{
    void* new_block = realloc(block, new_size);
    if (new_block)
    {
        _memory_trace.allocated += (new_size - old_size);
        _memory_trace.peak = tb_max64(_memory_trace.peak, _memory_trace.allocated);
    }
    return new_block;
}

static void DebugAllocatorFree(void* block, size_t size)
{
    if (size > _memory_trace.allocated)
        MINIMAL_WARN("Freeing %llu untraced bytes", size - _memory_trace.allocated);

    free(block);
    _memory_trace.allocated -= size;
}

static tb_allocator debug_allocator =
{
    .malloc = DebugAllocatorAlloc,
    .realloc = DebugAllocatorRealloc,
    .free = DebugAllocatorFree,
};

void* DebugMalloc(size_t size)               { return tb_mem_malloc(&debug_allocator, size); }
void* DebugRealloc(void* block, size_t size) { return tb_mem_realloc(&debug_allocator, block, size); }
void  DebugFree(void* block)                 { tb_mem_free(&debug_allocator, block); }

tb_allocator* DebugMemoryGetAllocator() { return &debug_allocator; }
size_t        DebugMemoryGetBytes()     { return _memory_trace.allocated; }
size_t        DebugMemoryGetPeak()      { return _memory_trace.peak; }

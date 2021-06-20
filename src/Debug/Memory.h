#ifndef DEBUG_MEMORY_H
#define DEBUG_MEMORY_H

#include "toolbox/tb_mem.h"

void* DebugMalloc(size_t size);
void* DebugRealloc(void* block, size_t size);
void  DebugFree(void* block);

tb_allocator* DebugMemoryGetAllocator();

size_t DebugMemoryGetBytes();
size_t DebugMemoryGetPeak();

#endif /* !DEBUG_MEMORY_H */
#ifndef DEBUG_MEMORY_H
#define DEBUG_MEMORY_H

#include "toolbox/tb_mem.h"

void* DebugMalloc(size_t size);
void* DebugRealloc(void* block, size_t size);
void* DebugMemDup(const void* src, size_t size);
void  DebugFree(void* block);
void  DebugConstFree(const void* block);

void* DebugStrDup(const char* src, size_t len);
void* DebugArrayAlloc(void* block, size_t size);

tb_allocator* DebugMemoryGetAllocator();

size_t DebugMemoryGetBytes();
size_t DebugMemoryGetPeak();

#endif /* !DEBUG_MEMORY_H */
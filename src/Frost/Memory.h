#ifndef FROST_MEMORY_H
#define FROST_MEMORY_H

#include "toolbox/tb_mem.h"

void* FrostMalloc(size_t size);
void* FrostRealloc(void* block, size_t size);
void* FrostMemDup(const void* src, size_t size);
void  FrostFree(void* block);
void  FrostConstFree(const void* block);

void* FrostArrayAlloc(void* block, size_t size);

tb_allocator* FrostMemoryGetAllocator();

size_t FrostMemoryGetBytes();
size_t FrostMemoryGetPeak();

#endif /* !FROST_MEMORY_H */
#ifndef FROST_MEM_H
#define FROST_MEM_H

#include "toolbox/tb_mem.h"

#define IGNIS_MALLOC(size)			FrostMalloc(size)
#define IGNIS_REALLOC(block,size)	FrostRealloc(block,size)
#define IGNIS_FREE(block)			FrostFree(block)

void* FrostMalloc(size_t size);
void* FrostRealloc(void* block, size_t size);
void  FrostFree(void* block);

tb_allocator* FrostGetAllocator();

size_t FrostMemGetBytes();
size_t FrostMemGetPeak();

#endif /* !FROST_MEM_H */

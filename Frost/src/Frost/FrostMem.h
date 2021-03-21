#ifndef FROST_MEM_H
#define FROST_MEM_H

#include "toolbox/tb_mem.h"

void* FrostMalloc(size_t size);
void FrostFree(void* block);

tb_allocator* FrostGetAllocator();

size_t FrostMemGetBytes();
size_t FrostMemGetPeak();

#endif /* !FROST_MEM_H */

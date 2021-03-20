#ifndef FROST_MEM_H
#define FROST_MEM_H

#include "toolbox/tb_mem.h"

tb_allocator* FrostGetAllocator();

size_t FrostGetMemBytes();

#endif /* !FROST_MEM_H */

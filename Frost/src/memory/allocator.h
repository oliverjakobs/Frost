#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdlib.h>

void* mem_alloc(void* allocator, size_t size);
void* mem_realloc(void* allocator, void* block, size_t size);
void mem_free(void* allocator, void* block, size_t size);

#endif /* !ALLOCATOR_H */

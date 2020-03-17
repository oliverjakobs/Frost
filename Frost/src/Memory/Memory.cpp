#include "Memory.hpp"

void* operator new(size_t size)
{
	// printf("Allocating %d bytes\n", (int)size);

	return malloc(size);
}

void operator delete(void* memory)
{
	free(memory);
}

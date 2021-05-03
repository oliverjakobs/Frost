#include "EcsCommon.h"

#include <stdlib.h>
#include <string.h>

static EcsEntityID _entity_counter = 0;

void EcsEntityResetIDCounter()
{
	_entity_counter = 0;
}

EcsEntityID EcsEntityGetNextID()
{
	return _entity_counter++;
}

void* EcsMemAlloc(size_t size)
{
	return malloc(size);
}

void* EcsMemRealloc(void* block, size_t size)
{
	return realloc(block, size);
}

void* EcsMemDup(const void* block, size_t size)
{
	void* dup = EcsMemAlloc(size);
	if (dup) memcpy(dup, block, size);

	return dup;
}

void EcsMemFree(void* block)
{
	free(block);
}

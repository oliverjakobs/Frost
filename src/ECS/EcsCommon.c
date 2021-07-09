#include "EcsCommon.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

static EcsEntityID _entity_counter = ECS_NULL_ENTITY + 1;

void EcsEntityResetIDCounter()   { _entity_counter = ECS_NULL_ENTITY + 1; }
EcsEntityID EcsEntityGetNextID() { return _entity_counter++; }

int EcsEntryFill(EcsEntry* comp, EcsEntityID entity, const void* data, size_t size)
{
    comp->entity = entity;
    comp->data = EcsMemDup(data, size);
    return comp->data != NULL;
}

void EcsEntryClear(EcsEntry* comp, EcsReleaseFunc release)
{
    if (release) release(comp->data);
    EcsMemFree(comp->data);
}

int EcsEntryCmpID(const EcsEntry* l, const EcsEntry* r)               { return l->entity - r->entity; }
int EcsEntryCmp(const EcsEntry* l, const EcsEntry* r, EcsCmpFunc cmp) { return cmp ? cmp(l->data, r->data) : l->entity - r->entity; }

static void* ecs_allocator;
static EcsMallocCallback  ecs_malloc;
static EcsReallocCallback ecs_realloc;
static EcsFreeCallback    ecs_free;

void EcsMemSetAllocator(void* allocator, EcsMallocCallback malloc, EcsReallocCallback realloc, EcsFreeCallback free)
{
    ecs_allocator = allocator;
    ecs_malloc = malloc;
    ecs_realloc = realloc;
    ecs_free = free;
}

void* EcsMemMalloc(size_t size)               { return ecs_malloc ? ecs_malloc(ecs_allocator, size) : malloc(size); }
void* EcsMemRealloc(void* block, size_t size) { return ecs_realloc ? ecs_realloc(ecs_allocator, block, size) : realloc(block, size); }
void  EcsMemFree(void* block)                 { ecs_free ? ecs_free(ecs_allocator, block) : free(block); }

void* EcsMemCalloc(size_t count, size_t size)
{ 
    if (!ecs_malloc) return calloc(count, size);

    void* block = ecs_malloc(ecs_allocator, count * size);
    return block ? memset(block, 0, count * size) : NULL;
}

void* EcsMemDup(const void* block, size_t size)
{
    void* new_block = EcsMemMalloc(size);
    if (new_block) memcpy(new_block, block, size);
    return new_block;
}

size_t EcsMemArrayGrow(void** arr, size_t elem_size, size_t cap)
{
    size_t new_cap = (*arr && cap > 0) ? (size_t)ceil(cap * ECS_ARRAY_GROWTH) : 1;
    *arr = EcsMemRealloc(*arr, new_cap);
    return *arr ? new_cap : 0;
}

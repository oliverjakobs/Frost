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

void* EcsMemMalloc(size_t size)               { return malloc(size); }
void* EcsMemCalloc(size_t count, size_t size) { return calloc(count, size); }
void* EcsMemRealloc(void* block, size_t size) { return realloc(block, size); }
void  EcsMemFree(void* block)                 { free(block); }

void* EcsMemDup(const void* block, size_t size)
{
    void* dup = EcsMemMalloc(size);
    if (dup) memcpy(dup, block, size);
    return dup;
}

void* EcsArrayGrow(void* arr, size_t elem_size, size_t* cap)
{
    size_t new_cap = (arr && *cap > 0) ? (size_t)ceil(*cap * ECS_ARRAY_GROWTH) : 1;
    arr = EcsMemRealloc(arr, new_cap);
    if (arr) *cap = new_cap;
    return arr;
}

#ifndef ECS_LIST_H
#define ECS_LIST_H

#include "EcsCommon.h"

#define ECS_LIST_GROWTH 2.0

typedef struct EcsListEntry EcsListEntry;

typedef struct
{
    EcsEntry* entries;
    size_t capacity;
    size_t count;
    size_t component_size;

    EcsReleaseFunc release;
    EcsCmpFunc cmp;
} EcsList;

int EcsListAlloc(EcsList* list, size_t elem_size, EcsReleaseFunc release, EcsCmpFunc cmp);
void EcsListFree(EcsList* list);

void EcsListClear(EcsList* list);

void* EcsListInsert(EcsList* list, EcsEntityID entity, const void* component);
void EcsListRemove(EcsList* list, EcsEntityID entity);

void* EcsListFind(const EcsList* list, EcsEntityID entity);

EcsEntityID EcsListEntityAt(const EcsList* list, size_t index);
void* EcsListComponentAt(const EcsList* list, size_t index);
size_t EcsListSize(const EcsList* list);

#endif /* !ECS_LIST_H */

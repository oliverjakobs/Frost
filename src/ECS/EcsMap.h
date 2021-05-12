#ifndef ECS_MAP_H
#define ECS_MAP_H

#include "EcsCommon.h"

typedef struct
{
    EcsEntry* table;
    size_t capacity;
    size_t count;
    size_t component_size;

    EcsReleaseFunc release;
} EcsMap;

int EcsMapAlloc(EcsMap* map, size_t component_size, size_t initial_size, EcsReleaseFunc release);
void EcsMapFree(EcsMap* map);

void EcsMapClear(EcsMap* map);

void* EcsMapInsert(EcsMap* map, EcsEntityID entity, const void* component);
void EcsMapRemove(EcsMap* map, EcsEntityID entity);

void* EcsMapFind(const EcsMap* map, EcsEntityID entity);

struct EcsMapIter;
typedef struct EcsMapIter EcsMapIter;

EcsMapIter* EcsMapIterator(const EcsMap* map);
EcsMapIter* EcsMapIterNext(const EcsMap* map, const EcsMapIter* it);
EcsMapIter* EcsMapIterRemove(EcsMap* map, const EcsMapIter* it);

EcsEntityID EcsMapIterKey(const EcsMapIter* it);
void*       EcsMapIterValue(const EcsMapIter* it);

#endif /* !ECS_MAP_H */


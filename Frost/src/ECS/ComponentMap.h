#ifndef ECS_COMPONENT_MAP_H
#define ECS_COMPONENT_MAP_H

#include "clib/hashmap.h"

#include "Entity.h"

typedef struct
{
	clib_hashmap map;
	size_t element_size;
} EcsComponentMap;

int EcsComponentMapAlloc(EcsComponentMap* map, size_t element_size, void (*free_func)(void*));
void EcsComponentMapFree(EcsComponentMap* map);

void EcsComponentMapClear(EcsComponentMap* map);

void* EcsComponentMapInsert(EcsComponentMap* map, EcsEntityID entity, void* component);
void EcsComponentMapRemove(EcsComponentMap* map, EcsEntityID entity);

void* EcsComponentMapFind(const EcsComponentMap* map, EcsEntityID entity);

struct EcsComponentMapIter;
typedef struct EcsComponentMapIter EcsComponentMapIter;

EcsComponentMapIter* EcsComponentMapIterator(const EcsComponentMap* map);
EcsComponentMapIter* EcsComponentMapIterNext(const EcsComponentMap* map, const EcsComponentMapIter* iter);

EcsEntityID EcsComponentMapIterKey(const EcsComponentMapIter* iter);
void* EcsComponentMapIterValue(const EcsComponentMapIter* iter);

#endif /* !ECS_COMPONENT_MAP_H */


#ifndef COMPONENT_MAP_H
#define COMPONENT_MAP_H

#include "clib/hashmap.h"

#include "Entity.h"

typedef struct
{
	clib_hashmap map;
} ComponentMap;

int ComponentMapAlloc(ComponentMap* map, void (*free_func)(void*));
void ComponentMapFree(ComponentMap* map);

void ComponentMapClear(ComponentMap* map);

void* ComponentMapInsert(ComponentMap* map, EntityID entity, void* component);
void ComponentMapRemove(ComponentMap* map, EntityID entity);

void* ComponentMapFind(const ComponentMap* map, EntityID entity);

struct ComponentMapIter;
typedef struct ComponentMapIter ComponentMapIter;

#define COMPONENT_MAP_ITERATE_FOR(map, iter) \
	for (ComponentMapIter* iter = ComponentMapIterator(map); iter; iter = ComponentMapIterNext(map, iter))

ComponentMapIter* ComponentMapIterator(const ComponentMap* map);
ComponentMapIter* ComponentMapIterNext(const ComponentMap* map, const ComponentMapIter* iter);

EntityID ComponentMapIterKey(const ComponentMapIter* iter);
void* ComponentMapIterValue(const ComponentMapIter* iter);

#endif /* !COMPONENT_MAP_H */


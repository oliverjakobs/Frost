#include "ComponentMap.h"

#include "clib/hash.h"

#include <string.h>
#include <stdlib.h>

static void* ComponentMapKeyAlloc(const void* key)
{
	size_t size = sizeof(EntityID);
	void* block = malloc(size);

	if (block)
		memcpy(block, key, size);

	return block;
}

static void ComponentMapKeyFree(void* block)
{
	free(block);
}

static int ComponentMapCmp(const void* left, const void* right)
{
	return *(EntityID*)left - *(EntityID*)right;
}

static size_t ComponentMapHash(const void* key)
{
	return clib_hash_uint32(*(uint32_t*)key);
}

int ComponentMapAlloc(ComponentMap* map, void (*free_func)(void*))
{
	if (clib_hashmap_alloc(&map->map, ComponentMapHash, ComponentMapCmp, 0) != CLIB_HASHMAP_OK)
		return 1;

	clib_hashmap_set_key_alloc_funcs(&map->map, ComponentMapKeyAlloc, ComponentMapKeyFree);
	clib_hashmap_set_value_alloc_funcs(&map->map, NULL, free_func);
	return 0;
}

void ComponentMapFree(ComponentMap* map)
{
	clib_hashmap_free(&map->map);
}

void ComponentMapClear(ComponentMap* map)
{
	clib_hashmap_clear(&map->map);
}

void* ComponentMapInsert(ComponentMap* map, EntityID entity, void* component)
{
	return clib_hashmap_insert(&map->map, &entity, component);
}

void ComponentMapRemove(ComponentMap* map, EntityID entity)
{
	clib_hashmap_remove(&map->map, &entity);
}

void* ComponentMapFind(const ComponentMap* map, EntityID entity)
{
	return clib_hashmap_find(&map->map, &entity);;
}

ComponentMapIter* ComponentMapIterator(const ComponentMap* map)
{
	return (ComponentMapIter*)clib_hashmap_iterator(&map->map);
}

ComponentMapIter* ComponentMapIterNext(const ComponentMap* map, const ComponentMapIter* iter)
{
	return (ComponentMapIter*)clib_hashmap_iter_next(&map->map, (clib_hashmap_iter*)iter);;
}

EntityID ComponentMapIterKey(const ComponentMapIter* iter)
{
	return *(EntityID*)clib_hashmap_iter_get_key((clib_hashmap_iter*)iter);
}

void* ComponentMapIterValue(const ComponentMapIter* iter)
{
	return clib_hashmap_iter_get_value((clib_hashmap_iter*)iter);
}

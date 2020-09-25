#include "ComponentMap.h"

#include <string.h>
#include <stdlib.h>

static void* EcsComponentMapKeyAlloc(const void* key)
{
	size_t size = sizeof(EcsEntityID);
	void* block = malloc(size);

	if (block)
		memcpy(block, key, size);

	return block;
}

static void EcsComponentMapKeyFree(void* block)
{
	free(block);
}

static int EcsComponentMapCmp(const void* left, const void* right)
{
	return *(EcsEntityID*)left - *(EcsEntityID*)right;
}

static size_t EcsComponentMapHash(const void* key)
{
	return tb_hash_uint32(*(uint32_t*)key);
}

int EcsComponentMapAlloc(EcsComponentMap* map, size_t element_size, void (*free_func)(void*))
{
	if (tb_hashmap_alloc(&map->map, EcsComponentMapHash, EcsComponentMapCmp, 0) != TB_HASHMAP_OK)
		return 0;

	map->element_size = element_size;

	tb_hashmap_set_key_alloc_funcs(&map->map, EcsComponentMapKeyAlloc, EcsComponentMapKeyFree);
	tb_hashmap_set_value_alloc_funcs(&map->map, NULL, free_func);
	return 1;
}

void EcsComponentMapFree(EcsComponentMap* map)
{
	tb_hashmap_free(&map->map);
}

void EcsComponentMapClear(EcsComponentMap* map)
{
	tb_hashmap_clear(&map->map);
}

void* EcsComponentMapInsert(EcsComponentMap* map, EcsEntityID entity, void* component)
{
	return tb_hashmap_insert(&map->map, &entity, component);
}

void EcsComponentMapRemove(EcsComponentMap* map, EcsEntityID entity)
{
	tb_hashmap_remove(&map->map, &entity);
}

void* EcsComponentMapFind(const EcsComponentMap* map, EcsEntityID entity)
{
	return tb_hashmap_find(&map->map, &entity);;
}

EcsComponentMapIter* EcsComponentMapIterator(const EcsComponentMap* map)
{
	return (EcsComponentMapIter*)tb_hashmap_iterator(&map->map);
}

EcsComponentMapIter* EcsComponentMapIterNext(const EcsComponentMap* map, const EcsComponentMapIter* iter)
{
	return (EcsComponentMapIter*)tb_hashmap_iter_next(&map->map, (tb_hashmap_iter*)iter);;
}

EcsEntityID EcsComponentMapIterKey(const EcsComponentMapIter* iter)
{
	return *(EcsEntityID*)tb_hashmap_iter_get_key((tb_hashmap_iter*)iter);
}

void* EcsComponentMapIterValue(const EcsComponentMapIter* iter)
{
	return tb_hashmap_iter_get_value((tb_hashmap_iter*)iter);
}

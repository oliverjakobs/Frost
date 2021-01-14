#include "EcsMap.h"

#include <string.h>
#include <stdlib.h>

static void* EcsMapKeyAlloc(const void* key)
{
	size_t size = sizeof(EcsEntityID);
	void* block = malloc(size);

	if (block) memcpy(block, key, size);

	return block;
}

static int EcsMapCmp(const void* left, const void* right)
{
	return *(EcsEntityID*)left - *(EcsEntityID*)right;
}

static size_t EcsMapHash(const void* key)
{
	return tb_hash_uint32(*(uint32_t*)key);
}

int EcsMapAlloc(EcsMap* map, size_t element_size, EcsFreeFunc free)
{
	if (tb_hashmap_alloc(&map->map, EcsMapHash, EcsMapCmp, 0) != TB_HASHMAP_OK)
		return 0;

	map->element_size = element_size;

	tb_hashmap_set_key_alloc_funcs(&map->map, EcsMapKeyAlloc, EcsMemFree);
	tb_hashmap_set_value_alloc_funcs(&map->map, NULL, free ? free : EcsMemFree);
	return 1;
}

void EcsMapFree(EcsMap* map)
{
	tb_hashmap_free(&map->map);
}

void EcsMapClear(EcsMap* map)
{
	tb_hashmap_clear(&map->map);
}

void* EcsMapInsert(EcsMap* map, EcsEntityID entity, const void* component)
{
	void* data = EcsMemDup(component, map->element_size);
	if (!data) return NULL;

	return tb_hashmap_insert(&map->map, &entity, data);
}

void EcsMapRemove(EcsMap* map, EcsEntityID entity)
{
	tb_hashmap_remove(&map->map, &entity);
}

void* EcsMapFind(const EcsMap* map, EcsEntityID entity)
{
	return tb_hashmap_find(&map->map, &entity);;
}

EcsMapIter* EcsMapIterator(const EcsMap* map)
{
	return (EcsMapIter*)tb_hashmap_iterator(&map->map);
}

EcsMapIter* EcsMapIterNext(const EcsMap* map, const EcsMapIter* iter)
{
	return (EcsMapIter*)tb_hashmap_iter_next(&map->map, (tb_hashmap_iter*)iter);;
}

EcsEntityID EcsMapIterKey(const EcsMapIter* iter)
{
	return *(EcsEntityID*)tb_hashmap_iter_get_key((tb_hashmap_iter*)iter);
}

void* EcsMapIterValue(const EcsMapIter* iter)
{
	return tb_hashmap_iter_get_value((tb_hashmap_iter*)iter);
}

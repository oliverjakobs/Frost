#include "EcsMap.h"

#include <string.h>
#include <stdlib.h>

#include "toolbox/tb_mem.h"

static int EcsMapAllocEntry(void* allocator, tb_hashmap_entry* entry, void* key, void* value)
{
	entry->key = EcsMemDup(key, sizeof(EcsEntityID));

	if (!entry->key) return 0;

	entry->value = value;

	return 1;
}

static void EcsMapFreeEntry(void* allocator, tb_hashmap_entry* entry)
{
	EcsMemFree(entry->key);
	EcsMemFree(entry->value);
}

static int EcsMapCmp(const void* left, const void* right) { return *(EcsEntityID*)left - *(EcsEntityID*)right; }
static size_t EcsMapHash(const void* key) { return tb_hash_uint32(*(uint32_t*)key); }

int EcsMapAlloc(EcsMap* map, size_t element_size, EcsFreeFunc free)
{
	map->map.allocator = NULL;
	map->map.alloc = NULL;
	map->map.free = NULL;
	map->map.entry_alloc = EcsMapAllocEntry;
	map->map.entry_free = EcsMapFreeEntry;
	if (tb_hashmap_init(&map->map, EcsMapHash, EcsMapCmp, 0) != TB_HASHMAP_OK)
		return 0;

	map->element_size = element_size;
	return 1;
}

void EcsMapFree(EcsMap* map)
{
	tb_hashmap_destroy(&map->map);
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

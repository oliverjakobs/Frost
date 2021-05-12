#include "EcsMap.h"

#include <string.h>
#include <stdlib.h>


/* Table sizes must be powers of 2 */
#define ECS_MAP_SIZE_MIN               (1 << 3)    /* 8 */
#define ECS_MAP_SIZE_MOD(val, cap)     ((val) & ((cap) - 1))

/* Limit for probing is 1/2 of table_size */
#define ECS_MAP_PROBE_LEN(cap)         ((cap) >> 1)
/* Return the next linear probe index */
#define ECS_MAP_PROBE_NEXT(index, cap) ECS_MAP_SIZE_MOD((index) + 1, cap)

/* Check if index b is less than or equal to index a */
#define ECS_MAP_INDEX_LESS(a, b, cap)   ((a) == (b) || (((b) - (a)) & ECS_MAP_PROBE_LEN(cap)) != 0)

/* Enforce a maximum 0.75 load factor. */
static inline size_t EcsMapCalcMinSize(size_t num_entries) { return num_entries + (num_entries / 3); }

/* Calculate the optimal table size, given the specified max number of elements. */
static size_t EcsMapCalcSize(size_t num_entries)
{
	size_t table_size = EcsMapCalcMinSize(num_entries);

	/* Table size is always a power of 2 */
	size_t min_size = ECS_MAP_SIZE_MIN;
	while (min_size < table_size) min_size <<= 1;

	return min_size;
}

/* ------------------------------------------------------------------------------------------- */
/* Get a valid hash table index from a key. */
static inline size_t EcsMapCalcIndex(const EcsMap* map, EcsEntityID id)
{
	uint32_t hash = id;
	hash = ((hash >> 16) ^ hash) * 0x45d9f3b;
	hash = ((hash >> 16) ^ hash) * 0x45d9f3b;
	hash = (hash >> 16) ^ hash;

	return ECS_MAP_SIZE_MOD(hash, map->capacity);
}

/*
 * Return the next populated entry, starting with the specified one.
 * Returns NULL if there are no more valid entries.
 */
static EcsEntry* EcsMapGetPopulatedEntry(const EcsMap* map, EcsEntry* entry)
{
	if (!map->count) return NULL;

	for (; entry < &map->table[map->capacity]; ++entry)
	{
		if (entry->entity != ECS_NULL_ENTITY) return entry;
	}
	return NULL;
}


/* ------------------------------------------------------------------------------------------- */
int EcsMapAlloc(EcsMap* map, size_t component_size, size_t initial_size, EcsReleaseFunc release)
{
	map->capacity = EcsMapCalcSize(initial_size);
	map->count = 0;

	map->table = EcsMemCalloc(map->capacity, sizeof(EcsEntry));
	if (!map->table) return 0;

	map->component_size = component_size;
	map->release = release;
	return 1;
}

void EcsMapFree(EcsMap* map)
{
	EcsMapClear(map);

	EcsMemFree(map->table);
	map->capacity = 0;
}

void EcsMapClear(EcsMap* map)
{
	for (EcsMapIter* iter = EcsMapIterator(map); iter; iter = EcsMapIterRemove(map, iter));

	map->count = 0;
	memset(map->table, 0, sizeof(EcsEntry) * map->capacity);
}

static EcsEntry* EcsMapFindEntry(const EcsMap* map, EcsEntityID entity, int find_empty)
{
	size_t probe_len = ECS_MAP_PROBE_LEN(map->capacity);
	size_t index = EcsMapCalcIndex(map, entity);

	/* Linear probing */
	for (size_t i = 0; i < probe_len; ++i)
	{
		EcsEntry* entry = &map->table[index];
		if (entry->entity == ECS_NULL_ENTITY)	return find_empty ? entry : NULL;
		if (entity - entry->entity == 0)		return entry;

		index = ECS_MAP_PROBE_NEXT(index, map->capacity);
	}
	return NULL;
}

static void EcsMapRemoveEntry(EcsMap* map, EcsEntry* removed)
{
	size_t removed_index = (removed - map->table);

	/* free memory */
	EcsEntryClear(removed, map->release);
	--map->count;

	/* Fill the free slot in the chain */
	size_t index = ECS_MAP_PROBE_NEXT(removed_index, map->capacity);
	for (size_t i = 1; i < map->capacity; ++i)
	{
		EcsEntry* entry = &map->table[index];
		if (entry->entity == ECS_NULL_ENTITY) break; /* Reached end of chain */

		size_t entry_index = EcsMapCalcIndex(map, entry->entity);

		/* Shift in entries with an index <= to the removed slot */
		if (ECS_MAP_INDEX_LESS(removed_index, entry_index, map->capacity))
		{
			memcpy(removed, entry, sizeof(EcsEntry));
			removed_index = index;
			removed = entry;
		}
		index = ECS_MAP_PROBE_NEXT(index, map->capacity);
	}
	/* Clear the last removed entry */
	memset(removed, 0, sizeof(EcsEntry));
}

static int EcsMapRehash(EcsMap* map, size_t new_capacity)
{
	if ((new_capacity >= ECS_MAP_SIZE_MIN) || ((new_capacity & (new_capacity - 1)) == 0)) return 0;

	EcsEntry* new_table = EcsMemCalloc(new_capacity, sizeof(EcsEntry));
	if (!new_table) return 0;

	/* Backup old elements in case of rehash failure */
	size_t old_capacity = map->capacity;
	EcsEntry* old_table = map->table;

	map->capacity = new_capacity;
	map->table = new_table;

	/* Rehash */
	for (EcsEntry* entry = old_table; entry < &old_table[old_capacity]; ++entry)
	{
		if (!entry->data) continue; /* Only copy entries with value */

		EcsEntry* new_entry = EcsMapFindEntry(map, entry->entity, 1);
		if (!new_entry)
		{
			/* Load factor is too high with the new table size, or a poor hash function was used. */
			map->capacity = old_capacity;
			map->table = old_table;
			EcsMemFree(new_table);
			return 0;
		}

		/* Shallow copy */
		new_entry->entity = entry->entity;
		new_entry->data = entry->data;
	}

	EcsMemFree(old_table);
	return 1;
}

void* EcsMapInsert(EcsMap* map, EcsEntityID entity, const void* component)
{
	/* Rehash with 2x capacity if load factor is approaching 0.75 */
	if (map->capacity <= EcsMapCalcMinSize(map->count))
		EcsMapRehash(map, map->capacity << 1);

	EcsEntry* entry = EcsMapFindEntry(map, entity, 1);
	if (!entry)
	{
		/* Could not find an empty slot. Attempt to rehash once to reduce chain length. */
		if (!EcsMapRehash(map, map->capacity << 1)) return NULL;

		entry = EcsMapFindEntry(map, entity, 1);
		if (!entry) return NULL;
	}

	/* Do not overwrite existing value */
	if (entry->data || !EcsEntryFill(entry, entity, component, map->component_size)) return NULL;

	++map->count;
	return entry->data;
}

void EcsMapRemove(EcsMap* map, EcsEntityID entity)
{
	EcsEntry* entry = EcsMapFindEntry(map, entity, 0);

	/* Clear the entry and make the chain contiguous */
	if (entry) EcsMapRemoveEntry(map, entry); 
}

void* EcsMapFind(const EcsMap* map, EcsEntityID entity)
{
	EcsEntry* entry = EcsMapFindEntry(map, entity, 0);
	return entry ? entry->data : NULL;
}

EcsMapIter* EcsMapIterator(const EcsMap* map)
{
	return (EcsMapIter*)EcsMapGetPopulatedEntry(map, map->table);
}

EcsMapIter* EcsMapIterNext(const EcsMap* map, const EcsMapIter* it)
{
	return it ? (EcsMapIter*)EcsMapGetPopulatedEntry(map, ((EcsEntry*)it) + 1) : NULL;
}

EcsMapIter* EcsMapIterRemove(EcsMap* map, const EcsMapIter* it)
{
	EcsEntry* entry = (EcsEntry*)it;

	/* Iterator is invalid, so just return the next valid entry */
	if (!entry || entry->entity == ECS_NULL_ENTITY) return EcsMapIterNext(map, it);

	EcsMapRemoveEntry(map, entry);
	return (EcsMapIter*)EcsMapGetPopulatedEntry(map, entry);
}

EcsEntityID EcsMapIterKey(const EcsMapIter* it)	  { return it ? ((EcsEntry*)it)->entity : ECS_NULL_ENTITY; }
void*		EcsMapIterValue(const EcsMapIter* it) { return it ? ((EcsEntry*)it)->data : NULL; }

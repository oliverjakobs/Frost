#include "EcsList.h"

#include <stdlib.h>
#include <string.h>

static int EcsListGrow(EcsList* list, size_t new_size)
{
	EcsEntry* entries = EcsMemRealloc(list->entries, new_size * sizeof(EcsEntry));
	if (!entries) return 0;

	list->entries = entries;
	list->capacity = new_size;
	return 1;
}

int EcsListAlloc(EcsList* list, size_t component_size, size_t initial, EcsReleaseFunc release, EcsCmpFunc cmp)
{
	list->entries = NULL;
	list->capacity = initial;
	list->count = 0;

	if (initial > 0)
	{
		list->entries = EcsMemCalloc(initial, sizeof(EcsEntry));
		if (!list->entries) return 0;
	}
	
	list->component_size = component_size;

	list->release = release;
	list->cmp = cmp;

	return 1;
}

void EcsListFree(EcsList* list)
{
	EcsListClear(list);
	EcsMemFree(list->entries);
	list->capacity = 0;

	list->component_size = 0;
	list->release = NULL;
	list->cmp = NULL;
}

void EcsListClear(EcsList* list)
{
	for (size_t i = 0; i < list->count; ++i)
		EcsEntryClear(&list->entries[i], list->release);

	list->count = 0;
}

void* EcsListInsert(EcsList* list, EcsEntityID entity, const void* component)
{
	if (!list) return NULL;

	/* list needs to grow */
	if (list->count >= list->capacity)
	{
		size_t capacity = (list->capacity > 0) ? (size_t)(list->capacity * ECS_LIST_GROWTH) : 1;
		if (!EcsListGrow(list, capacity))
			return NULL;
	}

	/* create new entry */
	EcsEntry entry;
	if (!EcsEntryFill(&entry, entity, component, list->component_size))
		return NULL;

	/* find location for the new element */
	size_t index = 0;
	while (index < list->count && EcsEntryCmp(&list->entries[index], &entry, list->cmp) < 0)
		++index;

	/* move entries back to make space for new element if index is not at the end */
	if (index < list->count)
	{
		size_t size = (list->count - index) * sizeof(EcsEntry);
		memcpy(list->entries + (index + 1), list->entries + index, size);
	}

	list->count++;
	return memcpy(list->entries + index, &entry, sizeof(EcsEntry));
}

static size_t EcsListFindIndex(const EcsList* list, EcsEntityID entity)
{
	/* special case if component is sorted by id */
	if (!list->cmp)
	{
		size_t size = sizeof(EcsEntry);
		EcsEntry dummy = { .entity = entity };
		EcsEntry* entry = bsearch(&dummy, list->entries, list->count, size, EcsEntryCmpID);
		return entry - list->entries;
	}

	for (size_t i = 0; i < list->count; ++i)
	{
		if (entity == list->entries[i].entity) return i;
	}
	return list->count;
}

void EcsListRemove(EcsList* list, EcsEntityID entity)
{
	if (!list) return;

	size_t index = EcsListFindIndex(list, entity);
	if (index >= list->count) return;

	EcsEntryClear(&list->entries[index], list->release);

	size_t size = (list->count - (index + 1)) * sizeof(EcsEntry);
	memcpy(list->entries + index, list->entries + (index + 1), size);

	list->count--;
}

void* EcsListFind(const EcsList* list, EcsEntityID entity)
{
	if (!list) return NULL;

	size_t index = EcsListFindIndex(list, entity);
	return (index < list->count) ? list->entries[index].data : NULL;
}

EcsEntityID EcsListEntityAt(const EcsList* list, size_t index)	{ return list->entries[index].entity; }
void* EcsListComponentAt(const EcsList* list, size_t index)		{ return list->entries[index].data; }
size_t EcsListSize(const EcsList* list)							{ return list->count; }

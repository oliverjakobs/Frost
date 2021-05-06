#include "EcsList.h"

#include <stdlib.h>
#include <string.h>

struct EcsListEntry
{
	EcsEntityID entity;
	void* component;
};

static int EcsListEntryFill(EcsListEntry* entry, const void* component, EcsEntityID entity, size_t size)
{
	entry->entity = entity;
	entry->component = EcsMemDup(component, size);

	return entry->component != NULL;
}

static void EcsListEntryClear(const EcsList* list, EcsListEntry* entry)
{
	if (list->free) list->free(entry->component);
	EcsMemFree(entry->component);
}

static int EcsListEntryCmp(const EcsListEntry* left, const EcsListEntry* right, EcsCmpFunc cmp)
{
	if (cmp) return cmp(left->component, right->component);
	return left->entity - right->entity;
}

static int EcsListGrow(EcsList* list, size_t new_size)
{
	EcsListEntry* entries = EcsMemRealloc(list->entries, new_size * sizeof(EcsListEntry));

	if (!entries) return 0;

	list->entries = entries;
	list->entry_capacity = new_size;
	return 1;
}

int EcsListAlloc(EcsList* list, size_t elem_size, EcsFreeFunc free, EcsCmpFunc cmp)
{
	list->entries = NULL;
	list->entry_count = 0;
	list->entry_capacity = 0;
	list->component_size = elem_size;
	list->free = free;
	list->cmp = cmp;

	return 1;
}

void EcsListFree(EcsList* list)
{
	EcsListClear(list);
	EcsMemFree(list->entries);
	list->entry_capacity = 0;

	list->component_size = 0;
	list->free = NULL;
	list->cmp = NULL;
}

void EcsListClear(EcsList* list)
{
	for (size_t i = 0; i < list->entry_count; ++i)
		EcsListEntryClear(list, &list->entries[i]);

	list->entry_count = 0;
}

void* EcsListInsert(EcsList* list, EcsEntityID entity, const void* component)
{
	/* list needs to grow */
	if (list->entry_count >= list->entry_capacity)
	{
		size_t capacity = (list->entry_capacity > 0) ? (size_t)(list->entry_capacity * ECS_LIST_GROWTH) : 1;
		if (!EcsListGrow(list, capacity))
			return NULL;
	}

	size_t index = 0;
	EcsListEntry entry;
	if (!EcsListEntryFill(&entry, component, entity, list->component_size))
		return NULL;

	/* find location for the new element */
	while (index < list->entry_count && EcsListEntryCmp(&list->entries[index], &entry, list->cmp) < 0)
		++index;

	/* move entries back to make space for new element if index is not at the end */
	if (index < list->entry_count)
	{
		size_t size = (list->entry_count - index) * sizeof(EcsListEntry);
		size_t dest_offset = (index + 1);
		size_t src_offset = index;

		memcpy(list->entries + dest_offset, list->entries + src_offset, size);
	}

	list->entry_count++;

	return memcpy(list->entries + index, &entry, sizeof(EcsListEntry));
}

static size_t EcsListFindIndex(const EcsList* list, EcsEntityID entity)
{
	/* TODO special case for id sorted lists */

	for (size_t i = 0; i < list->entry_count; ++i)
	{
		if (entity == list->entries[i].entity)
			return i;
	}
	return list->entry_count;
}

void EcsListRemove(EcsList* list, EcsEntityID entity)
{
	size_t index = EcsListFindIndex(list, entity);
	if (index >= list->entry_count) return;

	EcsListEntryClear(list, &list->entries[index]);

	size_t size = (list->entry_count - (index + 1)) * sizeof(EcsListEntry);
	size_t dest_offset = index;
	size_t src_offset = (index + 1);

	memcpy(list->entries + dest_offset, list->entries + src_offset, size);

	list->entry_count--;
}

void* EcsListFind(const EcsList* list, EcsEntityID entity)
{
	size_t index = EcsListFindIndex(list, entity);
	return (index < list->entry_count) ? list->entries[index].component : NULL;
}

EcsEntityID EcsListEntityAt(const EcsList* list, size_t index)	{ return list->entries[index].entity; }
void* EcsListComponentAt(const EcsList* list, size_t index)		{ return list->entries[index].component; }
size_t EcsListSize(const EcsList* list)							{ return list->entry_count; }

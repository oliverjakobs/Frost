#include "ComponentList.h"

#include <stdlib.h>
#include <string.h>

int EcsComponentListAlloc(EcsComponentList* list, size_t elem_size, int (*cmp)(const void*, const void*))
{
	size_t initial_cap = 8;

	list->data = malloc(initial_cap * elem_size);

	if (!list->data) return 0;

	list->cap = initial_cap;
	list->len = 0;

	list->element_size = elem_size;

	list->cmp_func = cmp;

	return 1;
}

void EcsComponentListFree(EcsComponentList* list)
{
	free(list->data);
	list->cap = 0;
	list->len = 0;
	list->element_size = 0;

	list->cmp_func = NULL;
}

void EcsComponentListClear(EcsComponentList* list)
{
	list->len = 0;
}

int EcsComponentListResize(EcsComponentList* list, size_t size)
{
	char* data = realloc(list->data, size * list->element_size);

	if (!data) return 0;

	list->data = data;
	list->cap = size;
	return 1;
}

void* EcsComponentListInsert(EcsComponentList* list, void* component)
{
	/* list is full */
	if (list->len >= list->cap)
	{
		size_t size = (list->cap > 0) ? list->cap * 2 : 1;
		if (!EcsComponentListResize(list, size))
			return NULL;
	}

	size_t index = 0;

	/* find location for the new element */
	while (index < list->len && list->cmp_func(&list->data[index], component) < 0)
		++index;

	/* move entries back to make space for new element if index is not at the end */
	if (index < list->len)
	{
		size_t size = (list->len - index) * list->element_size;
		size_t dest_offset = (index + 1) * list->element_size;
		size_t src_offset = index * list->element_size;

		memcpy(list->data + dest_offset, list->data + src_offset, size);
	}

	list->len++;

	/* copy element into the list */
	size_t offset = index * list->element_size;
	return memcpy(list->data + offset, component, list->element_size);
}

void EcsComponentListRemove(EcsComponentList* list, size_t index)
{
	if (index >= list->len) return;

	size_t size = (list->len - (index + 1)) * list->element_size;
	size_t dest_offset = index * list->element_size;
	size_t src_offset = (index + 1) * list->element_size;

	memcpy(list->data + dest_offset, list->data + src_offset, size);
	list->len--;
}

void* EcsComponentListFind(const EcsComponentList* list, EcsEntityID entity)
{
	for (size_t i = 0; i < list->len; ++i)
	{
		void* component = &list->data[i];
		if (entity == *(EcsEntityID*)component)
			return component;
	}
	return NULL;
}

void* EcsComponentListAt(const EcsComponentList* list, size_t index)
{
	return list->data + index * list->element_size;
}

#include "ComponentList.h"

int ComponentListAlloc(ComponentList* list, size_t element_size, int (*cmp)(const void*, const void*))
{
	return clib_list_alloc(&list->list, 8, element_size, cmp) != CLIB_LIST_OK;
}

void ComponentListFree(ComponentList* list)
{
	clib_list_free(&list->list);
}

void ComponentListClear(ComponentList* list)
{
	clib_list_clear(&list->list);
}

void* ComponentListInsert(ComponentList* list, void* component)
{
	return clib_list_insert_and_grow(&list->list, component, 1.2f);
}

void ComponentListRemove(ComponentList* list, size_t index)
{
	clib_list_remove_at(&list->list, index);
}

void* ComponentListFind(ComponentList* list, EntityID entity)
{
	for (size_t i = 0; i < list->list.used; ++i)
	{
		void* component = clib_list_get(&list->list, i);
		if (entity == *(EntityID*)component)
			return component;
	}
	return NULL;
}

void* ComponentListAt(ComponentList* list, size_t index)
{
	return clib_list_get(&list->list, index);
}

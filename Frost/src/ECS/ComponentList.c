#include "ComponentList.h"

int EcsComponentListAlloc(EcsComponentList* list, size_t element_size, int (*cmp)(const void*, const void*))
{
	return tb_list_alloc(&list->list, 8, element_size, cmp) != TB_LIST_OK;
}

void EcsComponentListFree(EcsComponentList* list)
{
	tb_list_free(&list->list);
}

void EcsComponentListClear(EcsComponentList* list)
{
	tb_list_clear(&list->list);
}

size_t EcsComponentListLength(const EcsComponentList* list)
{
	return list->list.used;
}

void* EcsComponentListInsert(EcsComponentList* list, void* component)
{
	return tb_list_insert_and_grow(&list->list, component, 1.2f);
}

void EcsComponentListRemove(EcsComponentList* list, size_t index)
{
	tb_list_remove_at(&list->list, index);
}

void* EcsComponentListFind(const EcsComponentList* list, EcsEntityID entity)
{
	for (size_t i = 0; i < list->list.used; ++i)
	{
		void* component = tb_list_get(&list->list, i);
		if (entity == *(EcsEntityID*)component)
			return component;
	}
	return NULL;
}

void* EcsComponentListAt(const EcsComponentList* list, size_t index)
{
	return tb_list_get(&list->list, index);
}

#ifndef COMPONENT_LIST_H
#define COMPONENT_LIST_H

#include "clib/list.h"

#include "Entity.h"

typedef struct
{
	clib_list list;
} ComponentList;

int ComponentListAlloc(ComponentList* list, size_t element_size, int (*cmp)(const void*, const void*));
void ComponentListFree(ComponentList* list);

void ComponentListClear(ComponentList* list);

void* ComponentListInsert(ComponentList* list, void* component);
void ComponentListRemove(ComponentList* list, size_t index);

void* ComponentListFind(ComponentList* list, EntityID entity);
void* ComponentListAt(ComponentList* list, size_t index);

#define COMPONENT_LIST_ITERATE(component_list, index) \
	for (size_t index = 0; index < component_list->list.used; ++index)

#endif /* !COMPONENT_SET_H */
#ifndef COMPONENT_LIST_H
#define COMPONENT_LIST_H

#include "clib/list.h"

typedef struct
{
	clib_list list;
} ComponentList;

int ComponentListAlloc(ComponentList* list, size_t element_size, int (*cmp)(const void*, const void*));
void ComponentListFree(ComponentList* list);

void ComponentListClear(ComponentList* list);

void* ComponentListInsert(ComponentList* list, void* component);
void ComponentListRemove(ComponentList* list, size_t index);

void* ComponentListAt(ComponentList* list, size_t index);

#endif /* !COMPONENT_SET_H */

#ifndef ECS_COMPONENT_LIST_H
#define ECS_COMPONENT_LIST_H

#include "clib/list.h"

#include "Entity.h"

typedef struct
{
	clib_list list;
} EcsComponentList;

int EcsComponentListAlloc(EcsComponentList* list, size_t element_size, int (*cmp)(const void*, const void*));
void EcsComponentListFree(EcsComponentList* list);

void EcsComponentListClear(EcsComponentList* list);

size_t EcsComponentListLength(const EcsComponentList* list);

void* EcsComponentListInsert(EcsComponentList* list, void* component);
void EcsComponentListRemove(EcsComponentList* list, size_t index);

void* EcsComponentListFind(const EcsComponentList* list, EcsEntityID entity);
void* EcsComponentListAt(const EcsComponentList* list, size_t index);

#endif /* !ECS_COMPONENT_LIST_H */

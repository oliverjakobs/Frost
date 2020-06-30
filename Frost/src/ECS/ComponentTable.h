#ifndef COMPONENT_TABLE_H
#define COMPONENT_TABLE_H

#include "clib/hashset.h"

#include "Entity.h"
#include "Components.h"

typedef struct
{
	clib_hashset* table;
	size_t count;
} ComponentTable;

int ComponentTableInit(ComponentTable* components, size_t component_count);
void ComponentTableFree(ComponentTable* components);

int ComponentTableSetFreeFunc(ComponentTable* components, uint32_t type, void (*free_func)(void*));

void ComponentTableClear(ComponentTable* components);

void* ComponentTableAddComponent(ComponentTable* components, EntityID entity, uint32_t type, void* component);
void* ComponentTableGetComponent(ComponentTable* components, EntityID entity, uint32_t type);

void ComponentTableRemoveComponent(ComponentTable* components, EntityID entity, uint32_t type);
void ComponentTableRemoveEntity(ComponentTable* components, EntityID entity);

#endif /* !COMPONENT_TABLE_H */
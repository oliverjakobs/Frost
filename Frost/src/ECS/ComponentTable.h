#ifndef COMPONENT_TABLE_H
#define COMPONENT_TABLE_H

#include <stdint.h>

#include "clib/hashset.h"

#include "Components.h"

typedef int32_t EntityID;
#define NULL_ENTITY (-1)

typedef struct
{
	clib_hashset table[NUM_COMPONENT_TYPES];
} ComponentTable;

void ComponentTableInit(ComponentTable* table, size_t initial_size);
void ComponentTableFree(ComponentTable* table);

void ComponentTableClear(ComponentTable* table);

void* ComponentTableAddComponent(ComponentTable* table, EntityID entity, ComponentType type, void* component);
void* ComponentTableGetComponent(ComponentTable* table, EntityID entity, ComponentType type);

void ComponentTableRemoveComponent(ComponentTable* table, EntityID entity, ComponentType type);
void ComponentTableRemoveEntity(ComponentTable* table, EntityID entity);

#endif /* !COMPONENT_TABLE_H */
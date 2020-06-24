#ifndef ECS_COMPONENT_TABLE_H
#define ECS_COMPONENT_TABLE_H

#include "clib/hashmap.h"

#include "Components.h"

typedef struct
{
	clib_hashmap table[NUM_COMPONENT_TYPES];
} ComponentTable;

void ComponentTableInit(ComponentTable* table, size_t initial_size);
void ComponentTableFree(ComponentTable* table);

void ComponentTableClear(ComponentTable* table);

void* ComponentTableAddComponent(ComponentTable* table, const char* entity, ComponentType type, void* component);
void* ComponentTableGetComponent(ComponentTable* table, const char* entity, ComponentType type);

void ComponentTableRemoveComponent(ComponentTable* table, const char* entity, ComponentType type);
void ComponentTableRemoveEntity(ComponentTable* table, const char* entity);


#endif /* !ECS_COMPONENT_TABLE_H */
#ifndef COMPONENT_TABLE_H
#define COMPONENT_TABLE_H

#include "clib/hashset.h"
#include "clib/array.h"

#include "Entity.h"
#include "Components.h"

typedef uint32_t ComponentType;

typedef struct
{
	clib_array table;
} ComponentTable;

int ComponentTableInit(ComponentTable* components, size_t component_count);
void ComponentTableFree(ComponentTable* components);

void ComponentTableClear(ComponentTable* components);

ComponentType ComponentTableRegisterDataComponent(ComponentTable* components, size_t element_size, size_t initial_count, void (*free_func)(void*));

void* ComponentTableAddDataComponent(ComponentTable* components, EntityID entity, ComponentType type, void* component);
void* ComponentTableGetDataComponent(ComponentTable* components, EntityID entity, ComponentType type);

void ComponentTableRemoveDataComponent(ComponentTable* components, EntityID entity, ComponentType type);
void ComponentTableRemoveEntity(ComponentTable* components, EntityID entity);

#endif /* !COMPONENT_TABLE_H */
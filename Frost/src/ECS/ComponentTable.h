#ifndef ECS_COMPONENT_TABLE_H
#define ECS_COMPONENT_TABLE_H

#include "clib/dictionary.h"
#include "Components.h"

typedef enum
{
	COMPONENT_POSITION = 0,
	COMPONENT_PHYSICS,
	COMPONENT_MOVEMENT,
	COMPONENT_TEXTURE,
	COMPONENT_ANIMATION,
	COMPONENT_CAMERA,
	COMPONENT_INTERACTION,
	NUM_COMPONENT_TYPES
} ComponentType;

typedef struct
{
	clib_dict components[NUM_COMPONENT_TYPES];
} ComponentTable;

void ComponentTableInit(ComponentTable* table, size_t initial_size);
void ComponentTableFree(ComponentTable* table);

void ComponentTableClear(ComponentTable* table);

void* ComponentTableAddComponent(ComponentTable* table, const char* entity, ComponentType type, void* component);
void* ComponentTableGetComponent(ComponentTable* table, const char* entity, ComponentType type);


#endif /* !ECS_COMPONENT_TABLE_H */
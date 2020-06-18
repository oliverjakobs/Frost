#ifndef ECS_COMPONENT_TABLE_H
#define ECS_COMPONENT_TABLE_H

#include "clib/dictionary.h"

#include "math/vec2.h"

typedef enum
{
	COMPONENT_TRANSFORM = 0,
	COMPONENT_PHYSICS,
	COMPONENT_MOVEMENT,
	COMPONENT_TEXTURE,
	COMPONENT_ANIMATION,
	COMPONENT_CAMERA,
	COMPONENT_INTERACTION,
	COMPONENT_INTERACTOR,
	NUM_COMPONENT_TYPES
} ComponentType;

typedef struct
{
	clib_hashmap components[NUM_COMPONENT_TYPES];
} ComponentTable;

void ComponentTableInit(ComponentTable* table, size_t initial_size);
void ComponentTableFree(ComponentTable* table);

void ComponentTableClear(ComponentTable* table);

void* ComponentTableAddComponent(ComponentTable* table, const char* entity, ComponentType type, void* component);
void* ComponentTableGetComponent(ComponentTable* table, const char* entity, ComponentType type);

/* Helper Functions */
void ComponentTableSetEntityPosition(ComponentTable* table, const char* entity, vec2 pos);
vec2 ComponentTableGetEntityPosition(ComponentTable* table, const char* entity);
vec2 ComponentTableGetEntityCenter(ComponentTable* table, const char* entity);

int ComponentTableGetZIndex(ComponentTable* table, const char* entity);


#endif /* !ECS_COMPONENT_TABLE_H */
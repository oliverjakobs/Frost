#ifndef ECS_ENTITY_H
#define ECS_ENTITY_H

#include "ComponentTable.h"

typedef struct
{
	char* name;
	char* template;

	ComponentTable* components;
} EcsEntity;

void EcsEntityLoad(EcsEntity* entity, const char* name, const char* template, ComponentTable* components);
void EcsEntityDestroy(EcsEntity* entity);

/* Helper Functions */
void EntitySetPosition(const char* entity, ComponentTable* components, vec2 pos);

vec2 EntityGetPosition(const char* entity, ComponentTable* components);
vec2 EntityGetCenter(const char* entity, ComponentTable* components);

void EntitySetZIndex(const char* entity, ComponentTable* components, int z_index);
int EntityGetZIndex(const char* entity, ComponentTable* components);

rect EntityGetRect(const char* entity, ComponentTable* components);

#endif /* !ECS_ENTITY_H */

#ifndef ECS_ENTITY_H
#define ECS_ENTITY_H

#include "ComponentTable.h"

typedef struct ecs_entity
{
	char* name;
	char* template;

	int z_index;
} EcsEntity;

void EcsEntityLoad(EcsEntity* entity, const char* name, const char* template);
void EcsEntityDestroy(EcsEntity* entity);

/* Helper Functions */
void EcsEntitySetPosition(const char* entity, ComponentTable* table, vec2 pos);
vec2 EcsEntityGetPosition(const char* entity, ComponentTable* table);
vec2 EcsEntityGetCenter(const char* entity, ComponentTable* table);

#endif /* !ECS_ENTITY_H */

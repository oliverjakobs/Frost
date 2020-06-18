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

#endif /* !ECS_ENTITY_H */

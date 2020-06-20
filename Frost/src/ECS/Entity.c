#include "Entity.h"

#include <stdlib.h>
#include <string.h>

void EcsEntityLoad(EcsEntity* entity, const char* name, const char* template, ComponentTable* components)
{
	entity->name = (char*)malloc(strlen(name));
	strcpy(entity->name, name);

	entity->template = (char*)malloc(strlen(template));
	strcpy(entity->template, template);

	entity->components = components;
}

void EcsEntityDestroy(EcsEntity* entity)
{
	free(entity->name);
	free(entity->template);
}

void EntitySetPosition(const char* entity, ComponentTable* components, vec2 pos)
{
	Transform* transform = ComponentTableGetComponent(components, entity, COMPONENT_TRANSFORM);
	if (transform)
	{
		transform->position = pos;
	}
}

vec2 EntityGetPosition(const char* entity, ComponentTable* components)
{
	Transform* transform = ComponentTableGetComponent(components, entity, COMPONENT_TRANSFORM);
	if (transform)
	{
		return transform->position;
	}

	return vec2_zero();
}

vec2 EntityGetCenter(const char* entity, ComponentTable* components)
{
	Transform* transform = ComponentTableGetComponent(components, entity, COMPONENT_TRANSFORM);

	if (transform)
	{
		vec2 center;
		center.x = transform->position.x;
		center.y = transform->position.y + (transform->size.y / 2.0f);

		return center;
	}

	return vec2_zero();
}

void EntitySetZIndex(const char* entity, ComponentTable* components, int z_index)
{
	Transform* transform = ComponentTableGetComponent(components, entity, COMPONENT_TRANSFORM);
	if (transform)
		transform->z_index = z_index;
}

int EntityGetZIndex(const char* entity, ComponentTable* components)
{
	Transform* transform = ComponentTableGetComponent(components, entity, COMPONENT_TRANSFORM);
	if (transform)
		return transform->z_index;

	return 0;
}

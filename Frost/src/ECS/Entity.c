#include "Entity.h"

void EcsEntityLoad(EcsEntity* entity, const char* name, const char* template)
{
	entity->name = (char*)malloc(strlen(name));
	strcpy(entity->name, name);

	entity->template = (char*)malloc(strlen(template));
	strcpy(entity->template, template);

	entity->z_index = 0;
}

void EcsEntityDestroy(EcsEntity* entity)
{
	free(entity->name);
	free(entity->template);

	entity->z_index = 0;
}

void EcsEntitySetPosition(const char* entity, ComponentTable* table, vec2 pos)
{
	EcsPhysicsComponent* physics = (EcsPhysicsComponent*)ComponentTableGetComponent(table, entity, COMPONENT_PHYSICS);

	if (physics && physics->body)
	{
		physics->body->position = vec2_add(pos, (vec2) { physics->body_x, physics->body_y });
		return;
	}

	EcsPositionComponent* position = (EcsPositionComponent*)ComponentTableGetComponent(table, entity, COMPONENT_POSITION);

	if (position)
	{
		position->x = pos.x;
		position->y = pos.y;
	}
}

vec2 EcsEntityGetPosition(const char* entity, ComponentTable* table)
{
	EcsPhysicsComponent* physics = (EcsPhysicsComponent*)ComponentTableGetComponent(table, entity, COMPONENT_PHYSICS);

	if (physics && physics->body)
	{
		return vec2_sub(physics->body->position, (vec2) { physics->body_x, physics->body_y });
	}

	EcsPositionComponent* position = (EcsPositionComponent*)ComponentTableGetComponent(table, entity, COMPONENT_POSITION);
	if (position)
	{
		return (vec2) { position->x, position->y };
	}

	return (vec2) { 0.0f, 0.0f };
}

vec2 EcsEntityGetCenter(const char* entity, ComponentTable* table)
{
	EcsPhysicsComponent* physics = (EcsPhysicsComponent*)ComponentTableGetComponent(table, entity, COMPONENT_PHYSICS);
	if (physics && physics->body)
	{
		return physics->body->position;
	}

	return EcsEntityGetPosition(entity, table);
}
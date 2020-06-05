#include "Entity.h"

void EcsEntityLoad(EcsEntity* entity, const char* name, const char* template)
{
	entity->name = (char*)malloc(strlen(name));
	strcpy(entity->name, name);

	entity->template = (char*)malloc(strlen(template));
	strcpy(entity->template, template);

	entity->z_index = 0;

	entity->position = NULL;
	entity->physics = NULL;
	entity->movement = NULL;
	entity->texture = NULL;
	entity->animation = NULL;
	entity->camera = NULL;
	entity->interaction = NULL;
}

void EcsEntityDestroy(EcsEntity* entity)
{
	free(entity->name);
	free(entity->template);

	entity->z_index = 0;

	entity->position = NULL;
	entity->physics = NULL;
	entity->movement = NULL;
	entity->texture = NULL;
	entity->animation = NULL;
	entity->camera = NULL;
	entity->interaction = NULL;
}

void EcsEntitySetPosition(EcsEntity* entity, vec2 pos)
{
	if (entity->physics && entity->physics->body)
	{
		entity->physics->body->position = vec2_add(pos, (vec2){ entity->physics->body_x, entity->physics->body_y });
	}

	if (entity->position)
	{
		entity->position->x = pos.x;
		entity->position->y = pos.y;
	}
}

vec2 EcsEntityGetPosition(EcsEntity* entity)
{
	if (entity->physics && entity->physics->body)
	{
		return vec2_sub(entity->physics->body->position, (vec2) { entity->physics->body_x, entity->physics->body_y });
	}

	if (entity->position)
	{
		return (vec2){ entity->position->x, entity->position->y };
	}

	return (vec2){ 0.0f, 0.0f };
}

vec2 EcsEntityGetCenter(EcsEntity* entity)
{
	if (entity->physics && entity->physics->body)
	{
		return entity->physics->body->position;
	}

	return EcsEntityGetPosition(entity);
}

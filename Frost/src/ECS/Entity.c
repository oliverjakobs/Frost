#include "Entity.h"

void EcsEntityLoad(EcsEntity* entity, const char* name)
{
	entity->name = (char*)malloc(strlen(name));
	strcpy(entity->name, name);

	entity->position = NULL;
	entity->physics = NULL;
	entity->movement = NULL;
	entity->texture = NULL;
}

void EcsEntityDestroy(EcsEntity* entity)
{
	free(entity->name);

	EcsEntityRemovePosition(entity);
	EcsEntityRemovePhysics(entity);
	EcsEntityRemoveMovement(entity);
	EcsEntityRemoveTexture(entity);
}

int EcsEntityAddPosition(EcsEntity* entity, float x, float y)
{
	if (entity->position) return 0;

	entity->position = (EcsPositionComponent*)malloc(sizeof(EcsPositionComponent));

	if (entity->position)
	{
		entity->position->x = x;
		entity->position->y = y;
		return 1;
	}

	return 0;
}

int EcsEntityAddPhysics(EcsEntity* entity, Body* body, float x, float y)
{
	if (entity->physics) return 0;

	entity->physics = (EcsPhysicsComponent*)malloc(sizeof(EcsPhysicsComponent));

	if (entity->physics)
	{
		entity->physics->body = body;
		entity->physics->body_x = x;
		entity->physics->body_y = y;
		return 1;
	}

	return 0;
}

int EcsEntityAddMovement(EcsEntity* entity, float ms, float jp)
{
	if (entity->movement) return 0;

	entity->movement = (EcsMovementComponent*)malloc(sizeof(EcsMovementComponent));

	if (entity->movement)
	{
		entity->movement->direction = DIRECTION_RIGHT;
		entity->movement->speed = ms;
		entity->movement->jump_power = jp;
		return 1;
	}

	return 0;
}

int EcsEntityAddTexture(EcsEntity* entity, IgnisTexture* texture, float width, float height, size_t frame)
{
	if (entity->texture) return 0;

	entity->texture = (EcsTextureComponent*)malloc(sizeof(EcsTextureComponent));

	if (entity->texture)
	{
		entity->texture->texture = texture;
		entity->texture->width = width;
		entity->texture->height = height;
		entity->texture->frame = frame;
		entity->texture->render_flip = RENDER_FLIP_NONE;
		return 1;
	}

	return 0;
}

void EcsEntityRemovePosition(EcsEntity* entity)
{
	if (entity->position)
	{
		free(entity->position);
		entity->position = NULL;
	}
}

void EcsEntityRemovePhysics(EcsEntity* entity)
{
	if (entity->physics)
	{
		free(entity->physics->body);
		free(entity->physics);
		entity->physics = NULL;
	}
}

void EcsEntityRemoveMovement(EcsEntity* entity)
{
	if (entity->movement)
	{
		free(entity->movement);
		entity->movement = NULL;
	}
}

void EcsEntityRemoveTexture(EcsEntity* entity)
{
	if (entity->texture)
	{
		free(entity->texture);
		entity->texture = NULL;
	}
}

void EcsEntitySetPosition(EcsEntity* entity, vec2 pos)
{
	if (entity->physics)
		entity->physics->body->position = vec2_add(pos, (vec2){ entity->physics->body_x, entity->physics->body_y });

	if (entity->position)
	{
		entity->position->x = pos.x;
		entity->position->y = pos.y;
	}
}

vec2 EcsEntityGetPosition(EcsEntity* entity)
{
	if (entity->physics)
		return vec2_sub(entity->physics->body->position, (vec2){ entity->physics->body_x, entity->physics->body_y });

	if (entity->position)
		return (vec2){ entity->position->x, entity->position->y };

	return (vec2){ 0.0f, 0.0f };
}

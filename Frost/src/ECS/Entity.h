#ifndef ECS_ENTITY_H
#define ECS_ENTITY_H

#include "ComponentList.h"

typedef struct ecs_entity
{
	char* name;
	char* template;

	int z_index;

	EcsPositionComponent* position;
	EcsPhysicsComponent* physics;
	EcsMovementComponent* movement;
	EcsTextureComponent* texture;
	EcsAnimationComponent* animation;
	EcsCameraComponent* camera;
	EcsInteractionComponent* interaction;
} EcsEntity;

void EcsEntityLoad(EcsEntity* entity, const char* name, const char* template);
void EcsEntityDestroy(EcsEntity* entity);

/* Helper Functions */
void EcsEntitySetPosition(EcsEntity* entity, vec2 pos);
vec2 EcsEntityGetPosition(EcsEntity* entity);
vec2 EcsEntityGetCenter(EcsEntity* entity);

#endif /* !ECS_ENTITY_H */

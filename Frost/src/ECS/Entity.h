#ifndef ECS_ENTITY_H
#define ECS_ENTITY_H

#include "Components.h"

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

int EcsEntityAddPosition(EcsEntity* entity, float x, float y);
int EcsEntityAddPhysics(EcsEntity* entity, Body* body, float x, float y);
int EcsEntityAddMovement(EcsEntity* entity, float ms, float jp);
int EcsEntityAddTexture(EcsEntity* entity, IgnisTexture2D* texture, float width, float height, size_t frame);
int EcsEntityAddAnimation(EcsEntity* entity, Animator* animator);
int EcsEntityAddCamera(EcsEntity* entity, float smooth);
int EcsEntityAddInteraction(EcsEntity* entity, float radius);

void EcsEntityRemovePosition(EcsEntity* entity);
void EcsEntityRemovePhysics(EcsEntity* entity);
void EcsEntityRemoveMovement(EcsEntity* entity);
void EcsEntityRemoveTexture(EcsEntity* entity);
void EcsEntityRemoveAnimation(EcsEntity* entity);
void EcsEntityRemoveCamera(EcsEntity* entity);
void EcsEntityRemoveInteraction(EcsEntity* entity);

/* Helper Functions */
void EcsEntitySetPosition(EcsEntity* entity, vec2 pos);
vec2 EcsEntityGetPosition(EcsEntity* entity);
vec2 EcsEntityGetCenter(EcsEntity* entity);

#endif /* !ECS_ENTITY_H */

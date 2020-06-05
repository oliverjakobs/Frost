#ifndef ECS_COMPONENT_LIST_H
#define ECS_COMPONENT_LIST_H

#include "Components.h"

typedef struct
{
	/* position */
	EcsPositionComponent* position;
	size_t position_used;
	size_t position_capacity;

	/* physics */
	EcsPhysicsComponent* physics;
	size_t physics_used;
	size_t physics_capacity;

	/* movement */
	EcsMovementComponent* movement;
	size_t movement_used;
	size_t movement_capacity;

	/* texture */
	EcsTextureComponent* texture;
	size_t texture_used;
	size_t texture_capacity;

	/* animataion */
	EcsAnimationComponent* animation;
	size_t animation_used;
	size_t animation_capacity;

	/* camera */
	EcsCameraComponent* camera;
	size_t camera_used;
	size_t camera_capacity;

	/* interaction */
	EcsInteractionComponent* interaction;
	size_t interaction_used;
	size_t interaction_capacity;
} EcsComponentList;

int EcsComponentListInit(EcsComponentList* list, size_t max_size);
void EcsComponentListDelete(EcsComponentList* list);

EcsPositionComponent* EcsComponentListGetNextPosition(EcsComponentList* list, float x, float y);
EcsPhysicsComponent* EcsComponentListGetNextPhysics(EcsComponentList* list, Body* body, float x, float y);
EcsMovementComponent* EcsComponentListGetNextMovement(EcsComponentList* list, float ms, float jp);
EcsTextureComponent* EcsComponentListGetNextTexture(EcsComponentList* list, IgnisTexture2D* texture, float width, float height, size_t frame);
EcsAnimationComponent* EcsComponentListGetNextAnimation(EcsComponentList* list, Animator* animator);
EcsCameraComponent* EcsComponentListGetNextCamera(EcsComponentList* list, float smooth);
EcsInteractionComponent* EcsComponentListGetNextInteraction(EcsComponentList* list, float radius);

#endif /* !ECS_COMPONENT_LIST_H */

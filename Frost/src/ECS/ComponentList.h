#ifndef ECS_COMPONENT_LIST_H
#define ECS_COMPONENT_LIST_H

#include "Components.h"

#include "clib/array.h"

typedef struct
{
	clib_array position;
	clib_array physics;
	clib_array movement;
	clib_array texture;
	clib_array animation;
	clib_array camera;
	clib_array interaction;
} EcsComponentList;

int EcsComponentListInit(EcsComponentList* list, size_t max_size);
void EcsComponentListDelete(EcsComponentList* list);

void EcsComponentListClear(EcsComponentList* list);

EcsPositionComponent* EcsComponentListGetNextPosition(EcsComponentList* list, float x, float y);
EcsPhysicsComponent* EcsComponentListGetNextPhysics(EcsComponentList* list, Body* body, float x, float y);
EcsMovementComponent* EcsComponentListGetNextMovement(EcsComponentList* list, float ms, float jp);
EcsTextureComponent* EcsComponentListGetNextTexture(EcsComponentList* list, IgnisTexture2D* texture, float width, float height, size_t frame);
EcsAnimationComponent* EcsComponentListGetNextAnimation(EcsComponentList* list, Animator* animator);
EcsCameraComponent* EcsComponentListGetNextCamera(EcsComponentList* list, float smooth);
EcsInteractionComponent* EcsComponentListGetNextInteraction(EcsComponentList* list, float radius);

#endif /* !ECS_COMPONENT_LIST_H */

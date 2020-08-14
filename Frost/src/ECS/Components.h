#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "Ecs.h"

#include "Components/Animator.h"
#include "Components/Transform.h"
#include "Components/RigidBody.h"
#include "Components/Sprite.h"
#include "Components/Movement.h"
#include "Components/CameraController.h"
#include "Components/Interaction.h"

typedef enum
{
	COMPONENT_TRANSFORM = 0,
	COMPONENT_RIGID_BODY,
	COMPONENT_MOVEMENT,
	COMPONENT_SPRITE,
	COMPONENT_ANIMATION,
	COMPONENT_CAMERA,
	COMPONENT_INTERACTION,
	COMPONENT_INTERACTOR
} DataComponentType;

#include "Components/Order.h"

typedef enum
{
	COMPONENT_TEMPLATE = 0,
	COMPONENT_Z_INDEX
} OrderComponentType;


/* Helper Functions */
EntityID EcsGetEntityAt(Ecs* ecs, vec2 pos);

void EcsSetEntityPosition(Ecs* ecs, EntityID entity, vec2 pos);

vec2 EcsGetEntityPosition(Ecs* ecs, EntityID entity);
vec2 EcsGetEntityCenter(Ecs* ecs, EntityID entity);

rect EcsGetEntityRect(Ecs* ecs, EntityID entity);


#endif /* !COMPONENTS_H */

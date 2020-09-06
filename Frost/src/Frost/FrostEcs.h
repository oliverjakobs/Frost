#ifndef FROST_ECS_H
#define FROST_ECS_H

#include "Ecs/Ecs.h"

/* ----------------------| SYSTEMS |------------------------------ */
#include "Systems/PhysicsSystem.h"
#include "Systems/PlayerSystem.h"
#include "Systems/AnimationSystem.h"
#include "Systems/InteractionSystem.h"

#include "Systems/RenderSystem.h"
#include "Systems/DebugRenderSystem.h"

/* ----------------------| COMPONENTS |--------------------------- */
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

/* ----------------------| UTILITY |------------------------------ */
EcsEntityID GetEntityAt(Ecs* ecs, vec2 pos);

void SetEntityPosition(Ecs* ecs, EcsEntityID entity, vec2 pos);

vec2 GetEntityPosition(Ecs* ecs, EcsEntityID entity);
vec2 GetEntityCenter(Ecs* ecs, EcsEntityID entity);

rect GetEntityRect(Ecs* ecs, EcsEntityID entity);

#endif /* !FROST_ECS_H */

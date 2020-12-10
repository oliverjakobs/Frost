#ifndef FROST_ECS_H
#define FROST_ECS_H

#include "Ecs/Ecs.h"

#include "Components/Animator.h"
#include "Components/Transform.h"
#include "Components/RigidBody.h"
#include "Components/Sprite.h"
#include "Components/Movement.h"
#include "Components/CameraController.h"
#include "Components/Inventory.h"
#include "Components/Interaction.h"

typedef enum
{
	COMPONENT_TRANSFORM,
	COMPONENT_RIGID_BODY,
	COMPONENT_MOVEMENT,
	COMPONENT_SPRITE,
	COMPONENT_ANIMATOR,
	COMPONENT_CAMERA,
	COMPONENT_INVENTORY,
	COMPONENT_INTERACTION,
	COMPONENT_INTERACTOR
} DataComponentType;

void RegisterDataComponents(Ecs* ecs);

#include "Components/Order.h"

typedef enum
{
	COMPONENT_TEMPLATE,
	COMPONENT_Z_INDEX
} OrderComponentType;

void RegisterOrderComponents(Ecs* ecs);

#include "Systems/PhysicsSystem.h"
#include "Systems/PlayerSystem.h"
#include "Systems/AnimationSystem.h"
#include "Systems/InventorySystem.h"
#include "Systems/InteractionSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/DebugRenderSystem.h"

typedef enum
{
	UPDATE_PHYSICS,
	UPDATE_PLAYER,
	UPDATE_ANIMATION,
	UPDATE_INVENTORY,
	UPDATE_INTERACTION
} UpdateSystemType;

void AddUpdateSystems(Ecs* ecs);

typedef enum
{
	RENDER_SPRITE,
	RENDER_DEBUG,
	RENDER_INVENTORY
} RenderSystemType;

void AddRenderSystems(Ecs* ecs);

/* ----------------------| Animations |------------------------------- */
typedef enum
{
	CONDITION_IDLE,
	CONDITION_FALL,
	CONDITION_WALK,
	CONDITION_JUMP
} AnimationCondition;

void RegisterAnimationCondition();

const char* AnimationConditionToString(AnimationCondition value);
AnimationCondition AnimationConditionFromString(const char* str);

/* ----------------------| Ecs utility |------------------------------ */
EcsEntityID GetEntityAt(Ecs* ecs, vec2 pos);

void SetEntityPosition(Ecs* ecs, EcsEntityID entity, vec2 pos);

vec2 GetEntityPosition(Ecs* ecs, EcsEntityID entity);
vec2 GetEntityCenter(Ecs* ecs, EcsEntityID entity);

rect GetEntityRect(Ecs* ecs, EcsEntityID entity);


#endif /* !FROST_ECS_H */

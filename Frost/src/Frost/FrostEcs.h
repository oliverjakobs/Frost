#ifndef FROST_ECS_H
#define FROST_ECS_H

#include "Ecs/Ecs.h"

#include "Components/Animator.h"
#include "Components/Transform.h"
#include "Components/EntityState.h"
#include "Components/RigidBody.h"
#include "Components/Sprite.h"
#include "Components/Movement.h"
#include "Components/CameraController.h"
#include "Components/Inventory.h"
#include "Components/Interaction.h"

#include "Components/Order.h"

typedef enum
{
	COMPONENT_TRANSFORM,
	COMPONENT_RIGID_BODY,
	COMPONENT_STATE,
	COMPONENT_MOVEMENT,
	COMPONENT_SPRITE,
	COMPONENT_ANIMATOR,
	COMPONENT_CAMERA,
	COMPONENT_INVENTORY,
	COMPONENT_INTERACTION,
	COMPONENT_INTERACTOR
} DataComponentType;

typedef enum
{
	COMPONENT_TEMPLATE,
	COMPONENT_Z_INDEX
} OrderComponentType;

#include "Systems/EntityStateSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/PlayerSystem.h"
#include "Systems/AnimationSystem.h"
#include "Systems/InventorySystem.h"
#include "Systems/InteractionSystem.h"
#include "Systems/RenderSystem.h"

typedef enum
{
	UPDATE_STATE,
	UPDATE_PHYSICS,
	UPDATE_PLAYER,
	UPDATE_ANIMATION,
	UPDATE_INVENTORY,
	UPDATE_INTERACTION
} UpdateSystemType;

typedef enum
{
	RENDER_SPRITE,
	RENDER_DEBUG,
	RENDER_INVENTORY
} RenderSystemType;

void LoadEcs(Ecs* ecs);

#endif /* !FROST_ECS_H */

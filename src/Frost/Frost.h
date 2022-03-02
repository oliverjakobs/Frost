#ifndef FROST_H
#define FROST_H

#include "Memory.h"
#include "SceneLoader.h"
#include "SceneSaver.h"

#include "Scenes/Scene.h"
#include "Scenes/SceneEditor.h"

#include "GUI/Gui.h"

#include "Minimal/Application.h"

int FrostLoad(MinimalApp* app, const char* config);
void FrostDestroy(MinimalApp* app);

int FrostLoadScene(Scene* scene, float w, float h, const char* start);

#include "Ecs/Ecs.h"

#include "Components/Animator.h"
#include "Components/Transform.h"
#include "Components/EntityState.h"
#include "Components/RigidBody.h"
#include "Components/Sprite.h"
#include "Components/Movement.h"
#include "Components/CameraController.h"
#include "Components/Player.h"
#include "Components/Inventory.h"
#include "Components/Interaction.h"
#include "Components/Item.h"

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
	COMPONENT_PLAYER,
	COMPONENT_INVENTORY,
	COMPONENT_INTERACTABLE,
	COMPONENT_ITEM
} DataComponentType;

typedef enum
{
	COMPONENT_TEMPLATE,
	COMPONENT_Z_INDEX
} OrderComponentType;

#include "Systems/PhysicsSystem.h"
#include "Systems/PlayerSystem.h"
#include "Systems/AnimationSystem.h"
#include "Systems/InventorySystem.h"
#include "Systems/InteractionSystem.h"
#include "Systems/RenderSystem.h"

typedef enum
{
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

int FrostLoadEcs(Ecs* ecs);

#endif /* !FROST_H */

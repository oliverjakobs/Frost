#ifndef FROST_H
#define FROST_H

#include "Memory.h"
#include "SceneLoader.h"
#include "SceneSaver.h"

#include "Scenes/Scene.h"
#include "editor/scene_editor.h"

#include "GUI/Gui.h"

#include "minimal/minimal.h"

int FrostLoad(MinimalApp* app, const char* config);

int FrostLoadGraphics(IgnisColorRGBA clear_color, GLenum blend_s, GLenum blend_d);
void FrostDestroyGraphics();

int FrostLoadGui(uint32_t w, uint32_t h, const char* font);
void FrostDestroyGui();

int FrostLoadScene(Scene* scene, uint32_t w, uint32_t h, const char* start);

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

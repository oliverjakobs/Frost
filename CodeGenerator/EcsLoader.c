#include "EcsLoader.h"
#include <string.h>

void RegisterDataComponents()
{
	EcsRegisterDataComponent(sizeof(Transform), NULL);
	EcsRegisterDataComponent(sizeof(RigidBody), NULL);
	EcsRegisterDataComponent(sizeof(Movement), NULL);
	EcsRegisterDataComponent(sizeof(Sprite), NULL);
	EcsRegisterDataComponent(sizeof(Animator), AnimatorFree);
	EcsRegisterDataComponent(sizeof(CameraController), NULL);
	EcsRegisterDataComponent(sizeof(Inventory), InventoryFree);
	EcsRegisterDataComponent(sizeof(Interaction), NULL);
	EcsRegisterDataComponent(sizeof(Interactor), NULL);
}

void RegisterOrderComponents()
{
	EcsRegisterOrderComponent(sizeof(Template), TemplateCmp);
	EcsRegisterOrderComponent(sizeof(ZIndex), ZIndexCmp);
}

void AddUpdateSystems()
{
	EcsAddUpdateSystem(PhysicsSystem);
	EcsAddUpdateSystem(PlayerSystem);
	EcsAddUpdateSystem(AnimationSystem);
	EcsAddUpdateSystem(InventoryUpdateSystem);
	EcsAddUpdateSystem(InteractionSystem);
}

void AddRenderSystems()
{
	EcsAddRenderSystem(ECS_RENDER_STAGE_PRIMARY, RenderSystem);
	EcsAddRenderSystem(ECS_RENDER_STAGE_DEBUG, DebugRenderSystem);
	EcsAddRenderSystem(ECS_RENDER_STAGE_UI, InventoryRenderSystem);
}


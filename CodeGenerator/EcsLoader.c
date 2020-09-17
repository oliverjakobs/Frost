#include "EcsLoader.h"

void RegisterDataComponents(Ecs* ecs)
{
	EcsRegisterDataComponent(ecs, sizeof(Transform), NULL);
	EcsRegisterDataComponent(ecs, sizeof(RigidBody), NULL);
	EcsRegisterDataComponent(ecs, sizeof(Movement), NULL);
	EcsRegisterDataComponent(ecs, sizeof(Sprite), NULL);
	EcsRegisterDataComponent(ecs, sizeof(Animator), AnimatorFree);
	EcsRegisterDataComponent(ecs, sizeof(CameraController), NULL);
	EcsRegisterDataComponent(ecs, sizeof(Inventory), InventoryFree);
	EcsRegisterDataComponent(ecs, sizeof(Interaction), NULL);
	EcsRegisterDataComponent(ecs, sizeof(Interactor), NULL);
}

void RegisterOrderComponents(Ecs* ecs)
{
	EcsRegisterOrderComponent(ecs, sizeof(Template), TemplateCmp);
	EcsRegisterOrderComponent(ecs, sizeof(ZIndex), ZIndexCmp);
}

void AddUpdateSystems(Ecs* ecs)
{
	EcsAddUpdateSystem(ecs, PhysicsSystem);
	EcsAddUpdateSystem(ecs, PlayerSystem);
	EcsAddUpdateSystem(ecs, AnimationSystem);
	EcsAddUpdateSystem(ecs, InventoryUpdateSystem);
	EcsAddUpdateSystem(ecs, InteractionSystem);
}

void AddRenderSystems(Ecs* ecs)
{
	EcsAddRenderSystem(ecs, ECS_RENDER_STAGE_SPRITE, RenderSystem);
	EcsAddRenderSystem(ecs, ECS_RENDER_STAGE_DEBUG, DebugRenderSystem);
	EcsAddRenderSystem(ecs, ECS_RENDER_STAGE_UI, InventoryRenderSystem);
}


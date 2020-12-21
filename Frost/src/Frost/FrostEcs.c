#include "FrostEcs.h"

#include "ECS/Loader.h"

EcsDataComponentLoader data_comps[] =
{
	[COMPONENT_TRANSFORM] =		{ sizeof(Transform),		NULL },
	[COMPONENT_RIGID_BODY] =	{ sizeof(RigidBody),		NULL },
	[COMPONENT_STATE] =			{ sizeof(EntityState),		NULL },
	[COMPONENT_MOVEMENT] =		{ sizeof(Movement),			NULL },
	[COMPONENT_SPRITE] =		{ sizeof(Sprite),			NULL },
	[COMPONENT_ANIMATOR] =		{ sizeof(Animator),			AnimatorFree },
	[COMPONENT_CAMERA] =		{ sizeof(CameraController), NULL },
	[COMPONENT_INVENTORY] =		{ sizeof(Inventory),		InventoryFree },
	[COMPONENT_INTERACTION] =	{ sizeof(Interaction),		NULL },
	[COMPONENT_INTERACTOR] =	{ sizeof(Interactor),		NULL }
};

EcsOrderComponentLoader order_comps[] =
{
	[COMPONENT_TEMPLATE] =	{ sizeof(Template), NULL },
	[COMPONENT_Z_INDEX] =	{ sizeof(ZIndex),	ZIndexCmp }
};

EcsUpdateSystemLoader update_systems[] =
{
	[UPDATE_STATE] =		{ EntityStateSystem },
	[UPDATE_PHYSICS] =		{ PhysicsSystem },
	[UPDATE_PLAYER] =		{ PlayerSystem },
	[UPDATE_ANIMATION] =	{ AnimationSystem },
	[UPDATE_INVENTORY] =	{ InventoryUpdateSystem },
	[UPDATE_INTERACTION] =	{ InteractionSystem }
};

EcsRenderSystemLoader render_systems[] =
{
	[RENDER_SPRITE] =		{ ECS_RENDER_STAGE_PRIMARY, RenderSystem },
	[RENDER_DEBUG] =		{ ECS_RENDER_STAGE_DEBUG,	DebugRenderSystem },
	[RENDER_INVENTORY] =	{ ECS_RENDER_STAGE_UI,		InventoryRenderSystem }
};

int LoadEcs(Ecs* ecs)
{
	EcsLoadUpdateSystems(ecs, update_systems, ECS_SIZEOF_LOADER(update_systems));
	EcsLoadRenderSystems(ecs, render_systems, ECS_SIZEOF_LOADER(render_systems));

	EcsLoadDataComponents(ecs, data_comps, ECS_SIZEOF_LOADER(data_comps));
	EcsLoadOrderComponents(ecs, order_comps, ECS_SIZEOF_LOADER(order_comps));

	return 1;
}

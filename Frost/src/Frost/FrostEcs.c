#include "FrostEcs.h"

#include "ECS/EcsLoader.h"

EcsDataComponentLoader data_comps[] =
{
	[COMPONENT_TRANSFORM] =		{ sizeof(Transform),		NULL },
	[COMPONENT_RIGID_BODY] =	{ sizeof(RigidBody),		NULL },
	[COMPONENT_STATE] =			{ sizeof(EntityState),		NULL },
	[COMPONENT_MOVEMENT] =		{ sizeof(Movement),			NULL },
	[COMPONENT_SPRITE] =		{ sizeof(Sprite),			NULL },
	[COMPONENT_ANIMATOR] =		{ sizeof(Animator),			AnimatorFree },
	[COMPONENT_CAMERA] =		{ sizeof(CameraController),	NULL },
	[COMPONENT_PLAYER] =		{ sizeof(Player),			NULL },
	[COMPONENT_INVENTORY] =		{ sizeof(Inventory),		InventoryFree },
	[COMPONENT_INTERACTABLE] =	{ sizeof(Interactable),		NULL },
	[COMPONENT_ITEM] =			{ sizeof(Item),				NULL }
};

EcsOrderComponentLoader order_comps[] =
{
	[COMPONENT_TEMPLATE] =	{ sizeof(Template), TemplateFree,	NULL },
	[COMPONENT_Z_INDEX] =	{ sizeof(ZIndex),	NULL,			ZIndexCmp }
};

EcsUpdateSystem update_systems[] =
{
	[UPDATE_STATE] =		{ EntityStateSystem },
	[UPDATE_PHYSICS] =		{ PhysicsSystem },
	[UPDATE_PLAYER] =		{ PlayerSystem },
	[UPDATE_ANIMATION] =	{ AnimationSystem },
	[UPDATE_INVENTORY] =	{ InventoryUpdateSystem },
	[UPDATE_INTERACTION] =	{ InteractionSystem }
};

EcsRenderSystem render_systems[] =
{
	[RENDER_SPRITE] =		{ ECS_RENDER_STAGE_PRIMARY, RenderSystem },
	[RENDER_DEBUG] =		{ ECS_RENDER_STAGE_DEBUG,	DebugRenderSystem },
	[RENDER_INVENTORY] =	{ ECS_RENDER_STAGE_UI,		InventoryRenderSystem }
};

int LoadEcs(Ecs* ecs)
{
	size_t update_size = ECS_SIZEOF_LOADER(update_systems);
	size_t render_size = ECS_SIZEOF_LOADER(render_systems);

	size_t data_size = ECS_SIZEOF_LOADER(data_comps);
	size_t order_size = ECS_SIZEOF_LOADER(order_comps);

	EcsReserveSystems(ecs, update_size, render_size);
	EcsLoadUpdateSystems(ecs, update_systems, update_size);
	EcsLoadRenderSystems(ecs, render_systems, render_size);

	EcsReserveComponents(ecs, data_size, order_size);
	EcsLoadDataComponents(ecs, data_comps, data_size);
	EcsLoadOrderComponents(ecs, order_comps, order_size);

	return 1;
}

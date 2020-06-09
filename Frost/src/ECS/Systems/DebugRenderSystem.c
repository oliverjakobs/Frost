#include "DebugRenderSystem.h"
#include "Graphics/Renderer.h"

void EcsSystemDebugRenderPre(const float* mat_view_proj)
{
	Primitives2DStart(mat_view_proj);
}

void EcsSystemDebugRender(EcsEntity* entity, ComponentTable* components)
{
	vec2 pos = EcsEntityGetPosition(entity->name, components);
	vec2 cen = EcsEntityGetCenter(entity->name, components);

	Primitives2DRenderCircle(pos.x, pos.y, 2.0f, IGNIS_WHITE);

	EcsInteractionComponent* interaction = (EcsInteractionComponent*)ComponentTableGetComponent(components, entity->name, COMPONENT_INTERACTION);
	if (interaction)
	{
		Primitives2DRenderCircle(cen.x, cen.y, interaction->radius, IGNIS_WHITE);
	}

	EcsPhysicsComponent* physics = (EcsPhysicsComponent*)ComponentTableGetComponent(components, entity->name, COMPONENT_PHYSICS);
	if (physics && physics->body)
	{
		vec2 pos = vec2_sub(physics->body->position, physics->body->halfSize);
		vec2 dim = vec2_mult(physics->body->halfSize, 2.0f);
		Primitives2DRenderRect(pos.x, pos.y, dim.x, dim.y, physics->body->type == BODY_TYPE_DYNAMIC ? IGNIS_GREEN : IGNIS_WHITE);
	}
}

void EcsSystemDebugRenderPost()
{
	Primitives2DFlush();
}

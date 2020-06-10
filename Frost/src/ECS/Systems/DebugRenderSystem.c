#include "DebugRenderSystem.h"

#include "Graphics/Renderer.h"

void EcsSystemDebugRenderPre(const float* mat_view_proj)
{
	Primitives2DStart(mat_view_proj);
}

void EcsSystemDebugRender(ComponentTable* components, const char* entity)
{
	vec2 pos = ComponentTableGetEntityPosition(components, entity);
	vec2 cen = ComponentTableGetEntityCenter(components, entity);

	Primitives2DRenderCircle(pos.x, pos.y, 2.0f, IGNIS_WHITE);

	EcsInteractionComponent* interaction = ComponentTableGetComponent(components, entity, COMPONENT_INTERACTION);
	if (interaction)
	{
		Primitives2DRenderCircle(cen.x, cen.y, interaction->radius, IGNIS_WHITE);
	}

	EcsPhysicsComponent* physics = ComponentTableGetComponent(components, entity, COMPONENT_PHYSICS);
	if (physics)
	{
		vec2 pos = vec2_sub(physics->body.position, physics->body.halfSize);
		vec2 dim = vec2_mult(physics->body.halfSize, 2.0f);
		Primitives2DRenderRect(pos.x, pos.y, dim.x, dim.y, physics->body.type == BODY_TYPE_DYNAMIC ? IGNIS_GREEN : IGNIS_WHITE);
	}
}

void EcsSystemDebugRenderPost()
{
	Primitives2DFlush();
}

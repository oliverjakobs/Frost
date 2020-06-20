#include "DebugRenderSystem.h"

#include "Graphics/Renderer.h"

#include "../Entity.h"

void EcsSystemDebugRenderPre(const float* mat_view_proj)
{
	Primitives2DStart(mat_view_proj);
}

void EcsSystemDebugRender(ComponentTable* components, const char* entity)
{
	vec2 pos = EntityGetPosition(entity, components);
	vec2 cen = EntityGetCenter(entity, components);

	Primitives2DRenderCircle(pos.x, pos.y, 2.0f, IGNIS_WHITE);

	EcsInteractionComponent* interaction = ComponentTableGetComponent(components, entity, COMPONENT_INTERACTION);
	if (interaction)
	{
		Primitives2DRenderCircle(cen.x, cen.y, interaction->radius, IGNIS_WHITE);
	}

	RigidBody* body = ComponentTableGetComponent(components, entity, COMPONENT_RIGID_BODY);
	if (body)
	{
		vec2 pos = vec2_sub(body->position, body->half_size);
		vec2 dim = vec2_mult(body->half_size, 2.0f);
		Primitives2DRenderRect(pos.x, pos.y, dim.x, dim.y, body->type == RIGID_BODY_DYNAMIC ? IGNIS_GREEN : IGNIS_WHITE);
	}
}

void EcsSystemDebugRenderPost()
{
	Primitives2DFlush();
}

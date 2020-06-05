#include "DebugRenderSystem.h"
#include "Graphics/Renderer.h"

void EcsSystemDebugRenderPre(const float* mat_view_proj)
{
	Primitives2DStart(mat_view_proj);
}

void EcsSystemDebugRender(EcsEntity* entity)
{
	vec2 pos = EcsEntityGetPosition(entity);
	vec2 cen = EcsEntityGetCenter(entity);

	Primitives2DRenderCircle(pos.x, pos.y, 2.0f, IGNIS_WHITE);

	if (entity->interaction)
	{
		Primitives2DRenderCircle(cen.x, cen.y, entity->interaction->radius, IGNIS_WHITE);
	}

	if (entity->physics && entity->physics->body)
	{
		vec2 pos = vec2_sub(entity->physics->body->position, entity->physics->body->halfSize);
		vec2 dim = vec2_mult(entity->physics->body->halfSize, 2.0f);
		Primitives2DRenderRect(pos.x, pos.y, dim.x, dim.y, entity->physics->body->type == BODY_TYPE_DYNAMIC ? IGNIS_GREEN : IGNIS_WHITE);
	}
}

void EcsSystemDebugRenderPost()
{
	Primitives2DFlush();
}

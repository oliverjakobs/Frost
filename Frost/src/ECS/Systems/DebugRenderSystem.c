#include "DebugRenderSystem.h"
#include "Graphics/Renderer.h"

void EcsSystemDebugRenderPre(const float* mat_view_proj)
{
	Primitives2DStart(mat_view_proj);
}

void EcsSystemDebugRender(EcsEntity* entity)
{
	vec2 pos = EcsEntityGetCenter(entity);

	if (entity->interaction)
	{
		Primitives2DRenderCircle(pos.x, pos.y, entity->interaction->radius, IGNIS_WHITE);
	}
}

void EcsSystemDebugRenderPost()
{
	Primitives2DFlush();
}

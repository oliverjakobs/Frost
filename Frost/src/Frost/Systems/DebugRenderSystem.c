#include "DebugRenderSystem.h"

#include "Graphics/Renderer.h"
#include "Frost/FrostEcs.h"

void DebugRenderSystem(Ecs* ecs, const float* mat_view_proj)
{
	Primitives2DStart(mat_view_proj);

	EcsComponentMap* map = EcsGetComponentMap(ecs, COMPONENT_TRANSFORM);
	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		Transform* transform = EcsComponentMapIterValue(iter);

		vec2 pos = GetEntityPosition(ecs, EcsComponentMapIterKey(iter));
		Primitives2DRenderCircle(pos.x, pos.y, 2.0f, IGNIS_WHITE);
	}

	map = EcsGetComponentMap(ecs, COMPONENT_INTERACTION);
	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		Interaction* interaction = EcsComponentMapIterValue(iter);

		vec2 cen = GetEntityCenter(ecs, EcsComponentMapIterKey(iter));
		Primitives2DRenderCircle(cen.x, cen.y, interaction->radius, IGNIS_WHITE);
	}

	map = EcsGetComponentMap(ecs, COMPONENT_RIGID_BODY);
	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		RigidBody* body = EcsComponentMapIterValue(iter);

		vec2 pos = vec2_sub(body->position, body->half_size);
		vec2 dim = vec2_mult(body->half_size, 2.0f);
		Primitives2DRenderRect(pos.x, pos.y, dim.x, dim.y, body->type == RIGID_BODY_DYNAMIC ? IGNIS_GREEN : IGNIS_WHITE);
	}

	Primitives2DFlush();
}

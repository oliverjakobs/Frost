#include "DebugRenderSystem.h"

#include "Graphics/Renderer.h"

void DebugRenderSystem(Ecs* ecs, ComponentTable* components, const float* mat_view_proj)
{
	Primitives2DStart(mat_view_proj);

	CLIB_HASHSET_ITERATE_FOR(clib_array_get(&components->table, COMPONENT_TRANSFORM), iter)
	{
		Transform* transform = clib_hashset_iter_get_value(iter);

		vec2 pos = EntityGetPosition(clib_hashset_iter_get_key(iter), components);
		Primitives2DRenderCircle(pos.x, pos.y, 2.0f, IGNIS_WHITE);
	}

	CLIB_HASHSET_ITERATE_FOR(clib_array_get(&components->table, COMPONENT_INTERACTION), iter)
	{
		Interaction* interaction = clib_hashset_iter_get_value(iter);

		vec2 cen = EntityGetCenter(clib_hashset_iter_get_key(iter), components);
		Primitives2DRenderCircle(cen.x, cen.y, interaction->radius, IGNIS_WHITE);
	}


	CLIB_HASHSET_ITERATE_FOR(clib_array_get(&components->table, COMPONENT_RIGID_BODY), iter)
	{
		RigidBody* body = clib_hashset_iter_get_value(iter);

		vec2 pos = vec2_sub(body->position, body->half_size);
		vec2 dim = vec2_mult(body->half_size, 2.0f);
		Primitives2DRenderRect(pos.x, pos.y, dim.x, dim.y, body->type == RIGID_BODY_DYNAMIC ? IGNIS_GREEN : IGNIS_WHITE);
	}

	Primitives2DFlush();
}

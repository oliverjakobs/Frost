#include "DebugRenderSystem.h"

#include "Graphics/Renderer.h"
#include "Frost/FrostEcs.h"

void DebugRenderSystem(Ecs* ecs, const float* mat_view_proj)
{
	Primitives2DStart(mat_view_proj);

	/*
	CameraController* camController = ComponentMapIterValue(ComponentMapIterator(EcsGetComponentMap(ecs, COMPONENT_CAMERA)));

	if (camController)
		Primitives2DRenderCircle(camController->camera->position.x, camController->camera->position.y, 10.0f, IGNIS_WHITE);
	*/

	COMPONENT_MAP_ITERATE(EcsGetComponentMap(ecs, COMPONENT_TRANSFORM), iter)
	{
		Transform* transform = ComponentMapIterValue(iter);

		vec2 pos = GetEntityPosition(ecs, ComponentMapIterKey(iter));
		Primitives2DRenderCircle(pos.x, pos.y, 2.0f, IGNIS_WHITE);
	}

	COMPONENT_MAP_ITERATE(EcsGetComponentMap(ecs, COMPONENT_INTERACTION), iter)
	{
		Interaction* interaction = ComponentMapIterValue(iter);

		vec2 cen = GetEntityCenter(ecs, ComponentMapIterKey(iter));
		Primitives2DRenderCircle(cen.x, cen.y, interaction->radius, IGNIS_WHITE);
	}


	COMPONENT_MAP_ITERATE(EcsGetComponentMap(ecs, COMPONENT_RIGID_BODY), iter)
	{
		RigidBody* body = ComponentMapIterValue(iter);

		vec2 pos = vec2_sub(body->position, body->half_size);
		vec2 dim = vec2_mult(body->half_size, 2.0f);
		Primitives2DRenderRect(pos.x, pos.y, dim.x, dim.y, body->type == RIGID_BODY_DYNAMIC ? IGNIS_GREEN : IGNIS_WHITE);
	}

	Primitives2DFlush();
}

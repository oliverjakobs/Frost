#include "Ecs.h"

void EcsInit(Ecs* ecs, size_t update_systems, size_t render_systems)
{
	clib_array_alloc(&ecs->systems_update, update_systems, sizeof(EcsUpdateSystem));
	clib_array_alloc(&ecs->systems_render, render_systems, sizeof(EcsRenderSystem));
}

void EcsDestroy(Ecs* ecs)
{
	clib_array_free(&ecs->systems_update);
	clib_array_free(&ecs->systems_render);
}

void EcsAddUpdateSystem(Ecs* ecs, void(*update)(ComponentTable*, const char*, float))
{
	EcsUpdateSystem system;

	system.update = update;

	clib_array_push(&ecs->systems_update, &system);
}

void EcsAddRenderSystem(Ecs* ecs, void(*render)(ComponentTable*, const char*), void(*pre)(const float*), void(*post)())
{
	EcsRenderSystem system;

	system.render = render;
	system.pre = pre;
	system.post = post;

	clib_array_push(&ecs->systems_render, &system);
}

void EcsUpdate(Ecs* ecs, EcsEntity* entities, size_t count, ComponentTable* components, float deltatime)
{
	for (size_t i = 0; i < ecs->systems_update.used; ++i)
	{
		for (size_t j = 0; j < count; ++j)
			((EcsUpdateSystem*)clib_array_get(&ecs->systems_update, i))->update(components, entities[j].name, deltatime);
	}
}

void EcsRender(Ecs* ecs, EcsEntity* entities, size_t count, ComponentTable* components, const float* mat_view_proj)
{
	for (size_t i = 0; i < ecs->systems_render.used; ++i)
	{
		EcsRenderSystem* system = (EcsRenderSystem*)clib_array_get(&ecs->systems_render, i);

		if (system->pre)
			system->pre(mat_view_proj);

		for (size_t j = 0; j < count; ++j)
			system->render(components, entities[j].name);

		if (system->post)
			system->post();
	}
}

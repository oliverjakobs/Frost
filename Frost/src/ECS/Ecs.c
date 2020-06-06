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

void EcsAddUpdateSystem(Ecs* ecs, void (*update)(EcsEntity* entity, float deltatime))
{
	EcsUpdateSystem* system = (EcsUpdateSystem*)clib_array_get_next(&ecs->systems_update);

	if (system)
	{
		system->update = update;
	}
}

void EcsAddRenderSystem(Ecs* ecs, void(*render)(EcsEntity* entity), void(*pre)(const float* mat_view_proj), void(*post)())
{
	EcsRenderSystem* system = (EcsRenderSystem*)clib_array_get_next(&ecs->systems_render);

	if (system)
	{
		system->render = render;
		system->pre = pre;
		system->post = post;
	}
}

void EcsUpdate(Ecs* ecs, EcsEntity** entities, size_t count, float deltatime)
{
	for (size_t i = 0; i < ecs->systems_update.used; ++i)
	{
		for (size_t j = 0; j < count; ++j)
			((EcsUpdateSystem*)clib_array_get(&ecs->systems_update, i))->update(entities[j], deltatime);
	}
}

void EcsRender(Ecs* ecs, EcsEntity** entities, size_t count, const float* mat_view_proj)
{
	for (size_t i = 0; i < ecs->systems_render.used; ++i)
	{
		EcsRenderSystem* system = (EcsRenderSystem*)clib_array_get(&ecs->systems_render, i);

		if (system->pre)
			system->pre(mat_view_proj);

		for (size_t j = 0; j < count; ++j)
			system->render(entities[j]);

		if (system->post)
			system->post();
	}
}

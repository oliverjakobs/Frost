#include "Ecs.h"

CLIB_DYNAMIC_ARRAY_DEFINE_FUNCS(ecs_update, EcsUpdateSystem)
CLIB_DYNAMIC_ARRAY_DEFINE_FUNCS(ecs_render, EcsRenderSystem)

void EcsInit(Ecs* ecs, size_t initial_size)
{
	clib_dynamic_array_init(&ecs->systems_update, initial_size);
	clib_dynamic_array_init(&ecs->systems_render, initial_size);
}

void EcsDestroy(Ecs* ecs)
{
	clib_dynamic_array_free(&ecs->systems_update);

	for (size_t i = 0; i < ecs->systems_render.size; i++)
		free(ecs_render_dynamic_array_get(&ecs->systems_render, i));
	clib_dynamic_array_free(&ecs->systems_render);
}

void EcsAddUpdateSystem(Ecs* ecs, EcsUpdateSystem system)
{
	ecs_update_dynamic_array_push(&ecs->systems_update, system);
}

void EcsAddRenderSystem(Ecs* ecs, void(*render)(EcsEntity* entity), void(*pre)(const float* mat_view_proj), void(*post)())
{
	EcsRenderSystem* system = (EcsRenderSystem*)malloc(sizeof(EcsRenderSystem));

	if (system && render)
	{
		system->render = render;
		system->pre = pre;
		system->post = post;

		ecs_render_dynamic_array_push(&ecs->systems_render, system);
	}
}

void EcsUpdate(Ecs* ecs, EcsEntity** entities, size_t count, float deltatime)
{
	for (size_t i = 0; i < ecs->systems_update.size; ++i)
	{
		for (size_t j = 0; j < count; ++j)
			ecs_update_dynamic_array_get(&ecs->systems_update, i)(entities[j], deltatime);
	}
}

void EcsRender(Ecs* ecs, EcsEntity** entities, size_t count, const float* mat_view_proj)
{
	for (size_t i = 0; i < ecs->systems_render.size; ++i)
	{
		EcsRenderSystem* system = ecs_render_dynamic_array_get(&ecs->systems_render, i);

		if (system->pre)
			system->pre(mat_view_proj);

		for (size_t j = 0; j < count; ++j)
			system->render(entities[j]);

		if (system->post)
			system->post();
	}
}

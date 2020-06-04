#include "Ecs.h"

void EcsInit(Ecs* ecs, size_t update_systems, size_t render_systems)
{
	ecs->systems_update = (EcsUpdateSystem*)malloc(update_systems * sizeof(EcsUpdateSystem));
	ecs->systems_update_count = update_systems;
	ecs->systems_update_used = 0;

	ecs->systems_render = (EcsRenderSystem*)malloc(render_systems * sizeof(EcsRenderSystem));
	ecs->systems_render_count = render_systems;
	ecs->systems_render_used = 0;
}

void EcsDestroy(Ecs* ecs)
{
	free(ecs->systems_update);
	ecs->systems_update_count = 0;
	ecs->systems_update_used = 0;

	free(ecs->systems_render);
	ecs->systems_render_count = 0;
	ecs->systems_render_used = 0;
}

void EcsAddUpdateSystem(Ecs* ecs, void (*update)(EcsEntity* entity, float deltatime))
{
	if (update && ecs->systems_update_used < ecs->systems_update_count)
	{
		ecs->systems_update[ecs->systems_update_used].update = update;

		ecs->systems_update_used++;
	}
}

void EcsAddRenderSystem(Ecs* ecs, void(*render)(EcsEntity* entity), void(*pre)(const float* mat_view_proj), void(*post)())
{
	if (render && ecs->systems_render_used < ecs->systems_render_count)
	{
		ecs->systems_render[ecs->systems_render_used].render = render;
		ecs->systems_render[ecs->systems_render_used].pre = pre;
		ecs->systems_render[ecs->systems_render_used].post = post;

		ecs->systems_render_used++;
	}
}

void EcsUpdate(Ecs* ecs, EcsEntity** entities, size_t count, float deltatime)
{
	for (size_t i = 0; i < ecs->systems_update_used; ++i)
	{
		for (size_t j = 0; j < count; ++j)
			ecs->systems_update[i].update(entities[j], deltatime);
	}
}

void EcsRender(Ecs* ecs, EcsEntity** entities, size_t count, const float* mat_view_proj)
{
	for (size_t i = 0; i < ecs->systems_render_used; ++i)
	{
		EcsRenderSystem* system = &ecs->systems_render[i];

		if (system->pre)
			system->pre(mat_view_proj);

		for (size_t j = 0; j < count; ++j)
			system->render(entities[j]);

		if (system->post)
			system->post();
	}
}

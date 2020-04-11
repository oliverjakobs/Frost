#include "Ecs.h"

CLIB_VECTOR_DEFINE_FUNCS(ecs_update, EcsUpdateSystem)
CLIB_VECTOR_DEFINE_FUNCS(ecs_render, EcsRenderSystem)

void EcsInit(Ecs* ecs, size_t initial_size)
{
	clib_vector_init(&ecs->systems_update, initial_size);
	clib_vector_init(&ecs->systems_render, initial_size);
}

void EcsDestroy(Ecs* ecs)
{
	clib_vector_free(&ecs->systems_update);
	clib_vector_free(&ecs->systems_render);
}

void EcsAddUpdateSystem(Ecs* ecs, EcsUpdateSystem system)
{
	ecs_update_vector_push(&ecs->systems_update, system);
}

void EcsAddRenderSystem(Ecs* ecs, EcsRenderSystem system)
{
	ecs_render_vector_push(&ecs->systems_render, system);
}

void EcsUpdate(Ecs* ecs, EcsEntity* entity, float deltatime)
{
	for (size_t i = 0; i < ecs->systems_update.size; i++)
		ecs_update_vector_get(&ecs->systems_update, i)(entity, deltatime);
}

void EcsRender(Ecs* ecs, EcsEntity* entity)
{
	for (size_t i = 0; i < ecs->systems_render.size; i++)
		ecs_render_vector_get(&ecs->systems_render, i)(entity);
}

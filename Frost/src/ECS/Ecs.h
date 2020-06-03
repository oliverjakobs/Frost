#ifndef ECS_H
#define ECS_H

#include "ECS/Entity.h"
#include "clib/dynamic_array.h"

typedef void (EcsUpdateSystem)(EcsEntity* entity, float deltatime);

typedef struct
{
	void (*render)(EcsEntity* entity);
	void (*pre)(const float* mat_view_proj);
	void (*post)();
} EcsRenderSystem;

typedef struct
{
	clib_dynamic_array systems_update;
	clib_dynamic_array systems_render;
} Ecs;

void EcsInit(Ecs* ecs, size_t initial_size);
void EcsDestroy(Ecs* ecs);

void EcsAddUpdateSystem(Ecs* ecs, EcsUpdateSystem system);
void EcsAddRenderSystem(Ecs* ecs, void (*render)(EcsEntity* entity), void (*pre)(const float* mat_view_proj), void (*post)());

void EcsUpdate(Ecs* ecs, EcsEntity** entities, size_t count, float deltatime);
void EcsRender(Ecs* ecs, EcsEntity** entities, size_t count, const float* mat_view_proj);

#endif /* !ECS_H */

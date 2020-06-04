#ifndef ECS_H
#define ECS_H

#include "ECS/Entity.h"

typedef struct
{
	void (*update)(EcsEntity* entity, float deltatime);
} EcsUpdateSystem;

typedef struct
{
	void (*render)(EcsEntity* entity);
	void (*pre)(const float* mat_view_proj);
	void (*post)();
} EcsRenderSystem;

typedef struct
{
	/* update systems */
	EcsUpdateSystem* systems_update;
	size_t systems_update_count;
	size_t systems_update_used;

	/* render systems */
	EcsRenderSystem* systems_render;
	size_t systems_render_count;
	size_t systems_render_used;
} Ecs;

void EcsInit(Ecs* ecs, size_t update_systems, size_t render_systems);
void EcsDestroy(Ecs* ecs);

void EcsAddUpdateSystem(Ecs* ecs, void (*update)(EcsEntity* entity, float deltatime));
void EcsAddRenderSystem(Ecs* ecs, void (*render)(EcsEntity* entity), void (*pre)(const float* mat_view_proj), void (*post)());

void EcsUpdate(Ecs* ecs, EcsEntity** entities, size_t count, float deltatime);
void EcsRender(Ecs* ecs, EcsEntity** entities, size_t count, const float* mat_view_proj);

#endif /* !ECS_H */

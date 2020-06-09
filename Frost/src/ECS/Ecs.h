#ifndef ECS_H
#define ECS_H

#include "ECS/Entity.h"
#include "clib/array.h"

typedef struct
{
	void (*update)(EcsEntity*, ComponentTable*, float);
} EcsUpdateSystem;

typedef struct
{
	void (*render)(EcsEntity*, ComponentTable*);
	void (*pre)(const float*);
	void (*post)();
} EcsRenderSystem;

typedef struct
{
	clib_array systems_update;
	clib_array systems_render;
} Ecs;

void EcsInit(Ecs* ecs, size_t update_systems, size_t render_systems);
void EcsDestroy(Ecs* ecs);

void EcsAddUpdateSystem(Ecs* ecs, void (*update)(EcsEntity*, ComponentTable*, float));
void EcsAddRenderSystem(Ecs* ecs, void (*render)(EcsEntity*, ComponentTable*), void (*pre)(const float*), void (*post)());

void EcsUpdate(Ecs* ecs, EcsEntity* entities, size_t count, ComponentTable* components, float deltatime);
void EcsRender(Ecs* ecs, EcsEntity* entities, size_t count, ComponentTable* components, const float* mat_view_proj);

#endif /* !ECS_H */

#ifndef ECS_H
#define ECS_H

#include "Systems.h"
#include "Components.h"

#include "Entity.h"

#include "clib/array.h"

typedef struct
{
	clib_array systems_update;
	clib_array systems_render;
} Ecs;

void EcsInit(Ecs* ecs, size_t update_systems, size_t render_systems);
void EcsDestroy(Ecs* ecs);

void EcsAddUpdateSystem(Ecs* ecs, void (*update)(ComponentTable*, float));
void EcsAddRenderSystem(Ecs* ecs, void (*render)(ComponentTable*, const float*));

void EcsUpdate(Ecs* ecs, EcsEntity* entities, size_t count, ComponentTable* components, float deltatime);
void EcsRender(Ecs* ecs, EcsEntity* entities, size_t count, ComponentTable* components, const float* mat_view_proj);

#endif /* !ECS_H */

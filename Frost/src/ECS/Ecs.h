#ifndef ECS_H
#define ECS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "clib/vector.h"
#include "ECS/Entity.h"

typedef void (EcsUpdateSystem)(EcsEntity* entity, float deltatime);
typedef void (EcsRenderSystem)(EcsEntity* entity);

typedef struct
{
	clib_vector systems_update;
	clib_vector systems_render;
} Ecs;

void EcsInit(Ecs* ecs, size_t initial_size);
void EcsDestroy(Ecs* ecs);

void EcsAddUpdateSystem(Ecs* ecs, EcsUpdateSystem system);
void EcsAddRenderSystem(Ecs* ecs, EcsRenderSystem system);

void EcsUpdate(Ecs* ecs, EcsEntity* entity, float deltatime);
void EcsRender(Ecs* ecs, EcsEntity* entity);

#ifdef __cplusplus
}
#endif

#endif /* !ECS_H */

#ifndef ECS_H
#define ECS_H

#include "ComponentTable.h"

#include "Entity.h"

#include "clib/array.h"

typedef struct
{
	clib_array systems_update;
	clib_array systems_render;

	clib_array indexed_entities;
} Ecs;

typedef struct
{
	void (*update)(Ecs*,ComponentTable*,float);
} EcsUpdateSystem;

typedef struct
{
	void (*render)(Ecs*, ComponentTable*, const float*);
} EcsRenderSystem;

void EcsInit(Ecs* ecs, size_t update_systems, size_t render_systems);
void EcsDestroy(Ecs* ecs);

void EcsRemoveEntity(Ecs* ecs, const char* entity);
void EcsClearEntities(Ecs* ecs);

void EcsAddUpdateSystem(Ecs* ecs, void (*update)(Ecs*,ComponentTable*,float));
void EcsAddRenderSystem(Ecs* ecs, void (*render)(Ecs*,ComponentTable*,const float*));

void EcsUpdate(Ecs* ecs, ComponentTable* components, float deltatime);
void EcsRender(Ecs* ecs, ComponentTable* components, const float* mat_view_proj);

typedef struct
{
	int z_index;
	const char* entity;
} ZIndexedEntity;

void EcsAddIndexedEntity(Ecs* ecs, const char* entity, int z_index);
int EcsGetEntityIndex(Ecs* ecs, const char* entity);

#endif /* !ECS_H */

#ifndef ECS_H
#define ECS_H

#include "ComponentTable.h"

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
	void (*render)(Ecs*,ComponentTable*,const float*);
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
	EntityID entity;
} ZIndexedEntity;

void EcsAddIndexedEntity(Ecs* ecs, EntityID entity, int z_index);

/* Helper Functions */
int EcsGetEntityIndex(Ecs* ecs, EntityID entity);

EntityID EcsGetEntityAt(ComponentTable* components, vec2 pos);

void EntitySetPosition(EntityID entity, ComponentTable* components, vec2 pos);

vec2 EntityGetPosition(EntityID entity, ComponentTable* components);
vec2 EntityGetCenter(EntityID entity, ComponentTable* components);

rect EntityGetRect(EntityID entity, ComponentTable* components);

#endif /* !ECS_H */

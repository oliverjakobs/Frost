#ifndef ECS_H
#define ECS_H

#include "clib/array.h"

#include "ComponentMap.h"
#include "Components.h"

typedef uint32_t ComponentType;

typedef struct
{
	clib_array systems_update;
	clib_array systems_render;

	clib_array table;

	clib_array indexed_entities;
} Ecs;

typedef struct
{
	void (*update)(Ecs*,float);
} EcsUpdateSystem;

typedef struct
{
	void (*render)(Ecs*,const float*);
} EcsRenderSystem;

void EcsInit(Ecs* ecs, size_t update_systems, size_t render_systems, size_t component_count);
void EcsDestroy(Ecs* ecs);

void EcsClearEntities(Ecs* ecs);

void EcsAddUpdateSystem(Ecs* ecs, void (*update)(Ecs*,float));
void EcsAddRenderSystem(Ecs* ecs, void (*render)(Ecs*,const float*));

void EcsUpdate(Ecs* ecs, float deltatime);
void EcsRender(Ecs* ecs, const float* mat_view_proj);

ComponentMap* EcsGetComponentMap(Ecs* ecs, ComponentType type);

void EcsClearComponents(Ecs* ecs);

ComponentType EcsRegisterDataComponent(Ecs* ecs, size_t element_size, size_t initial_count, void (*free_func)(void*));

void* EcsAddDataComponent(Ecs* ecs, EntityID entity, ComponentType type, void* component);
void* EcsGetDataComponent(Ecs* ecs, EntityID entity, ComponentType type);

void EcsRemoveDataComponent(Ecs* ecs, EntityID entity, ComponentType type);
void EcsRemoveEntity(Ecs* ecs, EntityID entity);

typedef struct
{
	int z_index;
	EntityID entity;
} ZIndexedEntity;

void EcsAddIndexedEntity(Ecs* ecs, EntityID entity, int z_index);

/* Helper Functions */
int EcsGetEntityZIndex(Ecs* ecs, EntityID entity);

EntityID EcsGetEntityAt(Ecs* ecs, vec2 pos);

void EcsSetEntityPosition(Ecs* ecs, EntityID entity, vec2 pos);

vec2 EcsGetEntityPosition(Ecs* ecs, EntityID entity);
vec2 EcsGetEntityCenter(Ecs* ecs, EntityID entity);

rect EcsGetEntityRect(Ecs* ecs, EntityID entity);

#endif /* !ECS_H */

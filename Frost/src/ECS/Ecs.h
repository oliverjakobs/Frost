#ifndef ECS_H
#define ECS_H

#include "clib/array.h"

#include "ComponentMap.h"
#include "ComponentList.h"

#define ECS_DEFAULT_UPDATE_SYSTEM_COUNT		4
#define ECS_DEFAULT_RENDER_SYSTEM_COUNT		2
#define ECS_DEFAULT_DATA_COMPONENT_COUNT	8
#define ECS_DEFAULT_ORDER_COMPONENT_COUNT	2

#define ECS_ARRAY_GROWTH_FACTOR		1.2f

typedef uint32_t ComponentType;

typedef struct
{
	clib_array systems_update;
	clib_array systems_render;

	clib_array data_components;
	clib_array order_components;
} Ecs;

typedef struct
{
	void (*update)(Ecs*,float);
} EcsUpdateSystem;

typedef struct
{
	void (*render)(Ecs*,const float*);
} EcsRenderSystem;

void EcsInit(Ecs* ecs);
void EcsDestroy(Ecs* ecs);

void EcsClear(Ecs* ecs);

void EcsAddUpdateSystem(Ecs* ecs, void (*update)(Ecs*,float));
void EcsAddRenderSystem(Ecs* ecs, void (*render)(Ecs*,const float*));

void EcsUpdate(Ecs* ecs, float deltatime);
void EcsRender(Ecs* ecs, const float* mat_view_proj);

ComponentMap* EcsGetComponentMap(Ecs* ecs, ComponentType type);
ComponentList* EcsGetComponentList(Ecs* ecs, ComponentType type);

ComponentType EcsRegisterDataComponent(Ecs* ecs, size_t element_size, void (*free_func)(void*));

void* EcsAddDataComponent(Ecs* ecs, EntityID entity, ComponentType type, void* component);
void* EcsGetDataComponent(Ecs* ecs, EntityID entity, ComponentType type);

void EcsRemoveDataComponent(Ecs* ecs, EntityID entity, ComponentType type);

ComponentType EcsRegisterOrderComponent(Ecs* ecs, size_t  element_size, int (*cmp)(const void*, const void*));

void* EcsAddOrderComponent(Ecs* ecs, ComponentType type, void* component);
void* EcsGetOrderComponent(Ecs* ecs, size_t index, ComponentType type);

void EcsRemoveOrderComponent(Ecs* ecs, EntityID entity, ComponentType type);

void EcsRemoveEntity(Ecs* ecs, EntityID entity);

#endif /* !ECS_H */

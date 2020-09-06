#ifndef ECS_H
#define ECS_H

#include "clib/array.h"

#include "ComponentMap.h"
#include "ComponentList.h"

#include "Event/Event.h"

#define ECS_DEFAULT_EVENT_SYSTEM_COUNT			4
#define ECS_DEFAULT_UPDATE_SYSTEM_COUNT			4
#define ECS_DEFAULT_RENDER_SYSTEM_COUNT			1
#define ECS_DEFAULT_RENDER_DEBUG_SYSTEM_COUNT	1
#define ECS_DEFAULT_RENDER_UI_SYSTEM_COUNT		1

#define ECS_DEFAULT_DATA_COMPONENT_COUNT	8
#define ECS_DEFAULT_ORDER_COMPONENT_COUNT	2

#define ECS_ARRAY_GROWTH_FACTOR		1.2f

typedef uint32_t EcsComponentType;

typedef struct
{
	clib_array systems_event;
	clib_array systems_update;
	clib_array systems_render;
	clib_array systems_render_debug;
	clib_array systems_render_ui;

	clib_array data_components;
	clib_array order_components;
} Ecs;

typedef struct
{
	void (*handle)(Ecs*,Event);
} EcsEventSystem;

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

void EcsAddEventSystem(Ecs* ecs, void (*handle)(Ecs*, Event));
void EcsAddUpdateSystem(Ecs* ecs, void (*update)(Ecs*,float));
void EcsAddRenderSystem(Ecs* ecs, void (*render)(Ecs*, const float*));
void EcsAddRenderDebugSystem(Ecs* ecs, void (*render)(Ecs*, const float*));
void EcsAddRenderUISystem(Ecs* ecs, void (*render)(Ecs*,const float*));

void EcsOnEvent(Ecs* ecs, Event e);
void EcsOnUpdate(Ecs* ecs, float deltatime);
void EcsOnRender(Ecs* ecs, const float* mat_view_proj);
void EcsOnRenderDebug(Ecs* ecs, const float* mat_view_proj);
void EcsOnRenderUI(Ecs* ecs, const float* mat_view_proj);

EcsComponentMap* EcsGetComponentMap(Ecs* ecs, EcsComponentType type);
EcsComponentList* EcsGetComponentList(Ecs* ecs, EcsComponentType type);

EcsComponentType EcsRegisterDataComponent(Ecs* ecs, size_t element_size, void (*free_func)(void*));

void* EcsAddDataComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type, void* component);
void* EcsGetDataComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type);

void EcsRemoveDataComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type);

EcsComponentType EcsRegisterOrderComponent(Ecs* ecs, size_t  element_size, int (*cmp)(const void*, const void*));

void* EcsAddOrderComponent(Ecs* ecs, EcsComponentType type, void* component);
void* EcsGetOrderComponent(Ecs* ecs, size_t index, EcsComponentType type);

void EcsRemoveOrderComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type);

void EcsRemoveEntity(Ecs* ecs, EcsEntityID entity);

#endif /* !ECS_H */

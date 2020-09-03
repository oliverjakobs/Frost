#ifndef ECS_H
#define ECS_H

#include "clib/array.h"

#include "ComponentMap.h"
#include "ComponentList.h"

#include "Event/Event.h"
#include "Camera/Camera.h"

#define ECS_DEFAULT_EVENT_SYSTEM_COUNT			4
#define ECS_DEFAULT_UPDATE_SYSTEM_COUNT			4
#define ECS_DEFAULT_RENDER_SYSTEM_COUNT			1
#define ECS_DEFAULT_RENDER_DEBUG_SYSTEM_COUNT	1
#define ECS_DEFAULT_RENDER_UI_SYSTEM_COUNT		1

#define ECS_DEFAULT_DATA_COMPONENT_COUNT	8
#define ECS_DEFAULT_ORDER_COMPONENT_COUNT	2

#define ECS_ARRAY_GROWTH_FACTOR		1.2f

typedef uint32_t ComponentType;

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
	void (*render)(Ecs*,const Camera*);
} EcsRenderSystem;

void EcsInit(Ecs* ecs);
void EcsDestroy(Ecs* ecs);

void EcsClear(Ecs* ecs);

void EcsAddEventSystem(Ecs* ecs, void (*handle)(Ecs*, Event));
void EcsAddUpdateSystem(Ecs* ecs, void (*update)(Ecs*,float));
void EcsAddRenderSystem(Ecs* ecs, void (*render)(Ecs*, const Camera*));
void EcsAddRenderDebugSystem(Ecs* ecs, void (*render)(Ecs*, const Camera*));
void EcsAddRenderUISystem(Ecs* ecs, void (*render)(Ecs*,const Camera*));

void EcsOnEvent(Ecs* ecs, Event e);
void EcsOnUpdate(Ecs* ecs, float deltatime);
void EcsOnRender(Ecs* ecs, const Camera* camera);
void EcsOnRenderDebug(Ecs* ecs, const Camera* camera);
void EcsOnRenderUI(Ecs* ecs, const Camera* camera);

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

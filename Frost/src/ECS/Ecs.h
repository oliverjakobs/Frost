#ifndef ECS_H
#define ECS_H

#include "toolbox/tb_array.h"

#include "ComponentMap.h"
#include "ComponentList.h"

#include "Event/Event.h"

#define ECS_DEFAULT_EVENT_SYSTEM_COUNT			4
#define ECS_DEFAULT_UPDATE_SYSTEM_COUNT			4
#define ECS_DEFAULT_RENDER_SYSTEM_COUNT			3

#define ECS_DEFAULT_DATA_COMPONENT_COUNT	8
#define ECS_DEFAULT_ORDER_COMPONENT_COUNT	2

#define ECS_ARRAY_GROWTH_FACTOR		1.2f

typedef uint32_t EcsComponentType;

typedef struct
{
	tb_array systems_event;
	tb_array systems_update;
	tb_array systems_render;

	tb_array data_components;
	tb_array order_components;
} Ecs;

typedef struct
{
	void (*handle)(Ecs*,Event);
} EcsEventSystem;

typedef struct
{
	void (*update)(Ecs*,float);
} EcsUpdateSystem;

typedef enum
{
	ECS_RENDER_STAGE_PRIMARY,
	ECS_RENDER_STAGE_DEBUG,
	ECS_RENDER_STAGE_UI
} EcsRenderStage;

typedef struct
{
	void (*render)(Ecs*,const float*);
	EcsRenderStage stage;
} EcsRenderSystem;

void EcsInit(Ecs* ecs);
void EcsDestroy(Ecs* ecs);

void EcsClear(Ecs* ecs);

void EcsAddEventSystem(Ecs* ecs, void (*handle)(Ecs*, Event));
void EcsAddUpdateSystem(Ecs* ecs, void (*update)(Ecs*,float));
void EcsAddRenderSystem(Ecs* ecs, EcsRenderStage stage, void (*render)(Ecs*, const float*));

void EcsOnEvent(Ecs* ecs, Event e);
void EcsOnUpdate(Ecs* ecs, float deltatime);
void EcsOnRender(Ecs* ecs, EcsRenderStage stage, const float* mat_view_proj);

EcsComponentMap* EcsGetComponentMap(Ecs* ecs, EcsComponentType type);
EcsComponentList* EcsGetComponentList(Ecs* ecs, EcsComponentType type);

int EcsRegisterDataComponent(Ecs* ecs, size_t element_size, void (*free_func)(void*));

void* EcsAddDataComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type, void* component);
void* EcsGetDataComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type);

void EcsRemoveDataComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type);

int EcsRegisterOrderComponent(Ecs* ecs, size_t  element_size, int (*cmp)(const void*, const void*));

void* EcsAddOrderComponent(Ecs* ecs, EcsComponentType type, void* component);
void* EcsGetOrderComponent(Ecs* ecs, size_t index, EcsComponentType type);

void EcsRemoveOrderComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type);

void EcsRemoveEntity(Ecs* ecs, EcsEntityID entity);

#endif /* !ECS_H */

#ifndef ECS_H
#define ECS_H

#include "ComponentMap.h"
#include "ComponentList.h"

#include "Event/Event.h"

#define ECS_DEFAULT_EVENT_SYSTEM_COUNT		4
#define ECS_DEFAULT_UPDATE_SYSTEM_COUNT		4
#define ECS_DEFAULT_RENDER_SYSTEM_COUNT		3

#define ECS_DEFAULT_DATA_COMPONENT_COUNT	8
#define ECS_DEFAULT_ORDER_COMPONENT_COUNT	2

typedef uint32_t EcsComponentType;

typedef struct EcsEventSystem EcsEventSystem;
typedef struct EcsUpdateSystem EcsUpdateSystem;
typedef struct EcsRenderSystem EcsRenderSystem;

typedef enum
{
	ECS_RENDER_STAGE_PRIMARY,
	ECS_RENDER_STAGE_DEBUG,
	ECS_RENDER_STAGE_UI
} EcsRenderStage;

typedef struct
{
	EcsEventSystem* systems_event;
	EcsUpdateSystem* systems_update;
	EcsRenderSystem* systems_render;

	EcsComponentMap* data_components;
	EcsComponentList* order_components;
} Ecs;

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

void* EcsAddOrderComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type, void* component);

void EcsRemoveOrderComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type);

void EcsRemoveEntity(Ecs* ecs, EcsEntityID entity);

#endif /* !ECS_H */

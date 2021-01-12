#ifndef ECS_H
#define ECS_H

#include "ComponentMap.h"
#include "ComponentList.h"

#define ECS_COMPONENT_REQUIRE(Comp, ecs, var, iter, type) \
	Comp* var = EcsGetDataComponent(ecs, EcsComponentMapIterKey(iter), type); \
	if (!var) continue

#define ECS_COMPONENT_REQUIRE_NODE(Comp, ecs, var, iter, type) \
	Comp* var = EcsGetDataComponent(ecs, EcsComponentNodeEntity(iter), type); \
	if (!var) continue

#define ECS_COMPONENT_OPTIONAL(Comp, ecs, var, iter, type) \
	Comp* var = EcsGetDataComponent(ecs, EcsComponentMapIterKey(iter), type)

#define ECS_COMPONENT_OPTIONAL_NODE(Comp, ecs, var, iter, type) \
	Comp* var = EcsGetDataComponent(ecs, EcsComponentNodeEntity(iter), type)

typedef uint32_t EcsComponentType;

typedef struct Ecs Ecs;

typedef struct
{
	void (*update)(Ecs*, float);
} EcsUpdateSystem;

typedef enum
{
	ECS_RENDER_STAGE_PRIMARY,
	ECS_RENDER_STAGE_DEBUG,
	ECS_RENDER_STAGE_UI
} EcsRenderStage;

typedef struct
{
	EcsRenderStage stage;
	void (*render)(const Ecs*, const float*);
} EcsRenderSystem;

struct Ecs
{
	EcsUpdateSystem* systems_update;
	EcsRenderSystem* systems_render;

	EcsComponentMap* data_components;
	EcsComponentList* order_components;
};

void EcsInit(Ecs* ecs);
void EcsDestroy(Ecs* ecs);

void EcsReserveSystems(Ecs* ecs, size_t update, size_t render);
void EcsReserveComponents(Ecs* ecs, size_t data, size_t order);

void EcsClear(Ecs* ecs);

void EcsAddUpdateSystem(Ecs* ecs, void (*update)(Ecs*,float));
void EcsAddRenderSystem(Ecs* ecs, EcsRenderStage stage, void (*render)(const Ecs*, const float*));

void EcsOnUpdate(Ecs* ecs, float deltatime);
void EcsOnRender(const Ecs* ecs, EcsRenderStage stage, const float* mat_view_proj);

void EcsRemoveEntity(Ecs* ecs, EcsEntityID entity);

EcsComponentMap* EcsGetComponentMap(const Ecs* ecs, EcsComponentType type);
EcsComponentList* EcsGetComponentList(const Ecs* ecs, EcsComponentType type);

/* Data Components */
int EcsRegisterDataComponent(Ecs* ecs, size_t elem_size, void (*free)(void*));

void* EcsAddDataComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type, void* component);
void* EcsGetDataComponent(const Ecs* ecs, EcsEntityID entity, EcsComponentType type);

void EcsRemoveDataComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type);

/* Order Components */
int EcsRegisterOrderComponent(Ecs* ecs, size_t elem_size, void (*free)(void*), int (*cmp)(const void*, const void*));

void* EcsAddOrderComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type, void* component);
void* EcsGetOrderComponent(const Ecs* ecs, EcsEntityID entity, EcsComponentType type);

void EcsRemoveOrderComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type);

/* Memory */
void* EcsMemAlloc(size_t size);
void* EcsMemRealloc(void* block, size_t size);
void* EcsMemDup(const void* block, size_t size);
void EcsMemFree(void* block);

#endif /* !ECS_H */

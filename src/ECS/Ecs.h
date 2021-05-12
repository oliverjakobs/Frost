#ifndef ECS_H
#define ECS_H

#include "EcsMap.h"
#include "EcsList.h"

#define ECS_REQUIRE_MAP(Comp, ecs, var, iter, type) \
	Comp* var = EcsGetDataComponent(ecs, EcsMapIterKey(iter), type); \
	if (!var) continue

#define ECS_OPTIONAL_MAP(Comp, ecs, var, iter, type) \
	Comp* var = EcsGetDataComponent(ecs, EcsMapIterKey(iter), type)

#define ECS_REQUIRE_LIST(Comp, ecs, var, list, index, type) \
	Comp* var = EcsGetDataComponent(ecs, EcsListEntityAt(list, index), type); \
	if (!var) continue

#define ECS_OPTIONAL_LIST(Comp, ecs, var, list, index, type) \
	Comp* var = EcsGetDataComponent(ecs, EcsListEntityAt(list, index), type)

typedef struct
{
	EcsUpdateCallback update;
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
	EcsRenderCallback render;
} EcsRenderSystem;

struct Ecs
{
	EcsUpdateSystem* systems_update;
	EcsRenderSystem* systems_render;

	EcsMap* data_components;
	EcsList* order_components;
};

void EcsInit(Ecs* ecs);
void EcsDestroy(Ecs* ecs);

void EcsReserveSystems(Ecs* ecs, size_t update, size_t render);
void EcsReserveComponents(Ecs* ecs, size_t data, size_t order);

void EcsClear(Ecs* ecs);

void EcsAddUpdateSystem(Ecs* ecs, EcsUpdateCallback update);
void EcsAddRenderSystem(Ecs* ecs, EcsRenderStage stage, EcsRenderCallback render);

void EcsOnUpdate(Ecs* ecs, void* world, float deltatime);
void EcsOnRender(const Ecs* ecs, EcsRenderStage stage, const void* world, const float* mat_view_proj);

void EcsRemoveEntity(Ecs* ecs, EcsEntityID entity);

EcsMap* EcsGetComponentMap(const Ecs* ecs, EcsComponentType type);
EcsList* EcsGetComponentList(const Ecs* ecs, EcsComponentType type);

/* Data Components */
int EcsRegisterDataComponent(Ecs* ecs, size_t size, size_t initial, EcsReleaseFunc release);

void* EcsAddDataComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type, const void* component);
void* EcsGetDataComponent(const Ecs* ecs, EcsEntityID entity, EcsComponentType type);

void EcsRemoveDataComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type);

/* Order Components */
int EcsRegisterOrderComponent(Ecs* ecs, size_t size, size_t initial, EcsReleaseFunc release, EcsCmpFunc cmp);

void* EcsAddOrderComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type, const void* component);
void* EcsGetOrderComponent(const Ecs* ecs, EcsEntityID entity, EcsComponentType type);

void EcsRemoveOrderComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type);

#endif /* !ECS_H */

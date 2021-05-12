#include "Ecs.h"

#include <string.h>
#include "toolbox/tb_array.h"

void EcsInit(Ecs* ecs)
{
	ecs->systems_update = NULL;
	ecs->systems_render = NULL;

	ecs->data_components = NULL;
	ecs->order_components = NULL;
}

void EcsDestroy(Ecs* ecs)
{
	tb_array_free(ecs->systems_update);
	tb_array_free(ecs->systems_render);

	for (EcsMap* it = ecs->data_components; it != tb_array_last(ecs->data_components); it++)
		EcsMapFree(it);

	for (EcsList* it = ecs->order_components; it != tb_array_last(ecs->order_components); it++)
		EcsListFree(it);

	tb_array_free(ecs->data_components);
	tb_array_free(ecs->order_components);
}

void EcsReserveSystems(Ecs* ecs, size_t update, size_t render)
{
	tb_array_reserve(ecs->systems_update, update);
	tb_array_reserve(ecs->systems_render, render);
}

void EcsReserveComponents(Ecs* ecs, size_t data, size_t order)
{
	tb_array_reserve(ecs->data_components, data);
	tb_array_reserve(ecs->order_components, order);
}

void EcsClear(Ecs* ecs)
{
	for (EcsMap* it = ecs->data_components; it != tb_array_last(ecs->data_components); it++)
		EcsMapClear(it);

	for (EcsList* it = ecs->order_components; it != tb_array_last(ecs->order_components); it++)
		EcsListClear(it);

	EcsEntityResetIDCounter();
}

void EcsAddUpdateSystem(Ecs* ecs, EcsUpdateCallback update)
{
	tb_array_push(ecs->systems_update, ((EcsUpdateSystem) { update }));
}

void EcsAddRenderSystem(Ecs* ecs, EcsRenderStage stage, EcsRenderCallback render)
{
	tb_array_push(ecs->systems_render, ((EcsRenderSystem) { stage, render }));
}

void EcsOnUpdate(Ecs* ecs, void* world, float deltatime)
{
	for (EcsUpdateSystem* it = ecs->systems_update; it != tb_array_last(ecs->systems_update); it++)
		it->update(ecs, world, deltatime);
}

void EcsOnRender(const Ecs* ecs, EcsRenderStage stage, const void* world, const float* mat_view_proj)
{
	for (EcsRenderSystem* it = ecs->systems_render; it != tb_array_last(ecs->systems_render); it++)
		if (it->stage == stage) it->render(ecs, world, mat_view_proj);
}

void EcsRemoveEntity(Ecs* ecs, EcsEntityID entity)
{
	for (uint32_t i = 0; i < tb_array_len(ecs->data_components); ++i)
		EcsRemoveDataComponent(ecs, entity, i);

	for (uint32_t i = 0; i < tb_array_len(ecs->order_components); ++i)
		EcsRemoveOrderComponent(ecs, entity, i);
}

EcsMap* EcsGetComponentMap(const Ecs* ecs, EcsComponentType type)
{
	return type < tb_array_len(ecs->data_components) ? &ecs->data_components[type] : NULL;
}

EcsList* EcsGetComponentList(const Ecs* ecs, EcsComponentType type)
{
	return type < tb_array_len(ecs->order_components) ? &ecs->order_components[type] : NULL;
}

int EcsRegisterDataComponent(Ecs* ecs, size_t size, size_t initial, EcsReleaseFunc release)
{
	EcsMap comp;
	if (!EcsMapAlloc(&comp, size, initial, release)) return 0;

	tb_array_push(ecs->data_components, comp);
	return 1;
}

void* EcsAddDataComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type, const void* component)
{
	return EcsMapInsert(EcsGetComponentMap(ecs, type), entity, component);
}

void* EcsGetDataComponent(const Ecs* ecs, EcsEntityID entity, EcsComponentType type)
{
	return EcsMapFind(EcsGetComponentMap(ecs, type), entity);
}

void EcsRemoveDataComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type)
{
	EcsMapRemove(EcsGetComponentMap(ecs, type), entity);
}

int EcsRegisterOrderComponent(Ecs* ecs, size_t size, size_t initial, EcsReleaseFunc release, EcsCmpFunc cmp)
{
	EcsList comp;
	if (!EcsListAlloc(&comp, size, initial, release, cmp)) return 0;

	tb_array_push(ecs->order_components, comp);
	return 1;
}

void* EcsAddOrderComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type, const void* component)
{
	return EcsListInsert(EcsGetComponentList(ecs, type), entity, component);
}

void* EcsGetOrderComponent(const Ecs* ecs, EcsEntityID entity, EcsComponentType type)
{
	return EcsListFind(EcsGetComponentList(ecs, type), entity);
}

void EcsRemoveOrderComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type)
{
	EcsListRemove(EcsGetComponentList(ecs, type), entity);
}

#include "Ecs.h"

#include <string.h>
#include "toolbox/tb_stretchy.h"

struct EcsUpdateSystem
{
	void (*update)(Ecs*, float);
};

struct EcsRenderSystem
{
	void (*render)(Ecs*, const float*);
	EcsRenderStage stage;
};

void EcsInit(Ecs* ecs)
{
	ecs->systems_update = NULL;
	ecs->systems_render = NULL;

	ecs->data_components = NULL;
	ecs->order_components = NULL;

	tb_stretchy_reserve(ecs->systems_update, ECS_DEFAULT_UPDATE_SYSTEM_COUNT);
	tb_stretchy_reserve(ecs->systems_render, ECS_DEFAULT_RENDER_SYSTEM_COUNT);

	tb_stretchy_reserve(ecs->data_components, ECS_DEFAULT_DATA_COMPONENT_COUNT);
	tb_stretchy_reserve(ecs->order_components, ECS_DEFAULT_ORDER_COMPONENT_COUNT);
}

void EcsDestroy(Ecs* ecs)
{
	tb_stretchy_free(ecs->systems_update);
	tb_stretchy_free(ecs->systems_render);

	EcsClear(ecs);
	for (EcsComponentMap* it = ecs->data_components; it != tb_stretchy_last(ecs->data_components); it++)
		EcsComponentMapFree(it);

	for (EcsComponentList* it = ecs->order_components; it != tb_stretchy_last(ecs->order_components); it++)
		EcsComponentListFree(it);

	tb_stretchy_free(ecs->data_components);
	tb_stretchy_free(ecs->order_components);
}

void EcsClear(Ecs* ecs)
{
	for (EcsComponentMap* it = ecs->data_components; it != tb_stretchy_last(ecs->data_components); it++)
		EcsComponentMapClear(it);

	for (EcsComponentList* it = ecs->order_components; it != tb_stretchy_last(ecs->order_components); it++)
		EcsComponentListClear(it);

	EcsEntityResetIDCounter();
}

void EcsAddUpdateSystem(Ecs* ecs, void(*update)(Ecs*,float))
{
	tb_stretchy_push(ecs->systems_update, ((EcsUpdateSystem) { update }));
}

void EcsAddRenderSystem(Ecs* ecs, EcsRenderStage stage, void (*render)(Ecs*,const float*))
{
	tb_stretchy_push(ecs->systems_render, ((EcsRenderSystem) { render, stage }));
}

void EcsOnUpdate(Ecs* ecs, float deltatime)
{
	for (EcsUpdateSystem* it = ecs->systems_update; it != tb_stretchy_last(ecs->systems_update); it++)
		it->update(ecs, deltatime);
}

void EcsOnRender(Ecs* ecs, EcsRenderStage stage, const float* mat_view_proj)
{
	for (EcsRenderSystem* it = ecs->systems_render; it != tb_stretchy_last(ecs->systems_render); it++)
	{
		if (it->stage == stage)
			it->render(ecs, mat_view_proj);
	}
}

void EcsRemoveEntity(Ecs* ecs, EcsEntityID entity)
{
	for (uint32_t i = 0; i < tb_stretchy_len(ecs->data_components); ++i)
		EcsRemoveDataComponent(ecs, entity, i);

	for (uint32_t i = 0; i < tb_stretchy_len(ecs->order_components); ++i)
		EcsRemoveOrderComponent(ecs, entity, i);
}

EcsComponentMap* EcsGetComponentMap(Ecs* ecs, EcsComponentType type)
{
	return type < tb_stretchy_len(ecs->data_components) ? &ecs->data_components[type] : NULL;
}

EcsComponentList* EcsGetComponentList(Ecs* ecs, EcsComponentType type)
{
	return type < tb_stretchy_len(ecs->order_components) ? &ecs->order_components[type] : NULL;
}

int EcsRegisterDataComponent(Ecs* ecs, size_t element_size, void (*free_func)(void*))
{
	EcsComponentMap comp;
	if (EcsComponentMapAlloc(&comp, element_size, free_func ? free_func : EcsMemFree))
	{
		tb_stretchy_push(ecs->data_components, comp);
		return 1;
	}

	return 0;
}

void* EcsAddDataComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type, void* component)
{
	EcsComponentMap* map = EcsGetComponentMap(ecs, type);

	if (!map) return NULL;

	void* data = malloc(map->element_size);
	memcpy(data, component, map->element_size);

	return EcsComponentMapInsert(map, entity, data);
}

void* EcsGetDataComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type)
{
	return EcsComponentMapFind(EcsGetComponentMap(ecs, type), entity);
}

void EcsRemoveDataComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type)
{
	EcsComponentMapRemove(EcsGetComponentMap(ecs, type), entity);
}

int EcsRegisterOrderComponent(Ecs* ecs, size_t element_size, int (*cmp)(const void*, const void*))
{
	EcsComponentList comp;
	if (EcsComponentListAlloc(&comp, element_size, cmp))
	{
		tb_stretchy_push(ecs->order_components, comp);
		return 1;
	}

	return 0;
}

void* EcsAddOrderComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type, void* component)
{
	EcsComponentList* list = EcsGetComponentList(ecs, type);

	if (!list) return NULL;

	return EcsComponentListInsert(list, entity, component);
}

void* EcsGetOrderComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type)
{
	return EcsComponentListFind(EcsGetComponentList(ecs, type), entity);
}

void EcsRemoveOrderComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type)
{
	EcsComponentListRemove(EcsGetComponentList(ecs, type), entity);
}

void* EcsMemAlloc(size_t size)
{
	return malloc(size);
}

void* EcsMemRealloc(void* block, size_t size)
{
	return realloc(block, size);
}

void* EcsMemDup(const void* block, size_t size)
{
	void* dup = EcsMemAlloc(size);
	if (dup) memcpy(dup, block, size);

	return dup;
}

void EcsMemFree(void* block)
{
	free(block);
}
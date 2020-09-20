#include "Ecs.h"

#include <string.h>

void EcsInit(Ecs* ecs)
{
	tb_array_alloc(&ecs->systems_event, ECS_DEFAULT_EVENT_SYSTEM_COUNT, sizeof(EcsEventSystem));
	tb_array_alloc(&ecs->systems_update, ECS_DEFAULT_UPDATE_SYSTEM_COUNT, sizeof(EcsUpdateSystem));
	tb_array_alloc(&ecs->systems_render, ECS_DEFAULT_RENDER_SYSTEM_COUNT, sizeof(EcsRenderSystem));

	tb_array_alloc(&ecs->data_components, ECS_DEFAULT_DATA_COMPONENT_COUNT, sizeof(EcsComponentMap));
	tb_array_alloc(&ecs->order_components, ECS_DEFAULT_ORDER_COMPONENT_COUNT, sizeof(EcsComponentList));
}

void EcsDestroy(Ecs* ecs)
{
	tb_array_free(&ecs->systems_event);
	tb_array_free(&ecs->systems_update);
	tb_array_free(&ecs->systems_render);

	EcsClear(ecs);
	for (size_t i = 0; i < ecs->data_components.used; ++i)
		EcsComponentMapFree(tb_array_get(&ecs->data_components, i));
	tb_array_free(&ecs->data_components);

	for (size_t i = 0; i < ecs->order_components.used; ++i)
		EcsComponentListFree(tb_array_get(&ecs->order_components, i));
	tb_array_free(&ecs->order_components);
}

void EcsClear(Ecs* ecs)
{
	for (size_t i = 0; i < ecs->data_components.used; ++i)
		EcsComponentMapClear(tb_array_get(&ecs->data_components, i));

	for (size_t i = 0; i < ecs->order_components.used; ++i)
		EcsComponentListClear(tb_array_get(&ecs->order_components, i));

	EcsEntityResetIDCounter();
}

void EcsAddEventSystem(Ecs* ecs, void(*handle)(Ecs*, Event))
{
	EcsEventSystem system;
	system.handle = handle;

	tb_array_push_and_grow(&ecs->systems_event, &system, ECS_ARRAY_GROWTH_FACTOR);
}

void EcsAddUpdateSystem(Ecs* ecs, void(*update)(Ecs*,float))
{
	EcsUpdateSystem system;
	system.update = update;

	tb_array_push_and_grow(&ecs->systems_update, &system, ECS_ARRAY_GROWTH_FACTOR);
}

void EcsAddRenderSystem(Ecs* ecs, EcsRenderStage stage, void (*render)(Ecs*,const float*))
{
	EcsRenderSystem system;
	system.render = render;
	system.stage = stage;

	tb_array_push_and_grow(&ecs->systems_render, &system, ECS_ARRAY_GROWTH_FACTOR);
}

void EcsOnEvent(Ecs* ecs, Event e)
{
	for (size_t i = 0; i < ecs->systems_event.used; ++i)
		((EcsEventSystem*)tb_array_get(&ecs->systems_event, i))->handle(ecs, e);
}

void EcsOnUpdate(Ecs* ecs, float deltatime)
{
	for (size_t i = 0; i < ecs->systems_update.used; ++i)
		((EcsUpdateSystem*)tb_array_get(&ecs->systems_update, i))->update(ecs, deltatime);
}

void EcsOnRender(Ecs* ecs, EcsRenderStage stage, const float* mat_view_proj)
{
	for (size_t i = 0; i < ecs->systems_render.used; ++i)
	{
		EcsRenderSystem* system = tb_array_get(&ecs->systems_render, i);

		if (system->stage == stage)
			system->render(ecs, mat_view_proj);
	}
}

static void EcsComponentFree(void* block)
{
	free(block);
}

EcsComponentMap* EcsGetComponentMap(Ecs* ecs, EcsComponentType type)
{
	return tb_array_get(&ecs->data_components, type);
}

EcsComponentList* EcsGetComponentList(Ecs* ecs, EcsComponentType type)
{
	return tb_array_get(&ecs->order_components, type);
}

EcsComponentType EcsRegisterDataComponent(Ecs* ecs, size_t element_size, void (*free_func)(void*))
{
	EcsComponentMap comp;
	if ((EcsComponentMapAlloc(&comp, element_size, free_func ? free_func : EcsComponentFree) == 0)
		&& (tb_array_push_and_grow(&ecs->data_components, &comp, ECS_ARRAY_GROWTH_FACTOR)))
		return (EcsComponentType)ecs->data_components.used - 1;
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
	if (type >= ecs->data_components.used) return NULL;

	return EcsComponentMapFind(EcsGetComponentMap(ecs, type), entity);
}

void EcsRemoveDataComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type)
{
	if (type >= ecs->data_components.used) return;

	EcsComponentMapRemove(EcsGetComponentMap(ecs, type), entity);
}

EcsComponentType EcsRegisterOrderComponent(Ecs* ecs, size_t element_size, int (*cmp)(const void*, const void*))
{
	EcsComponentList comp;
	if ((EcsComponentListAlloc(&comp, element_size, cmp) == 0)
		&& (tb_array_push_and_grow(&ecs->order_components, &comp, ECS_ARRAY_GROWTH_FACTOR)))
		return (EcsComponentType)ecs->order_components.used - 1;

	return 0;
}

void* EcsAddOrderComponent(Ecs* ecs, EcsComponentType type, void* component)
{
	EcsComponentList* list = EcsGetComponentList(ecs, type);

	if (!list) return NULL;

	return EcsComponentListInsert(list, component);
}

void* EcsGetOrderComponent(Ecs* ecs, size_t index, EcsComponentType type)
{
	if (type >= ecs->order_components.used) return NULL;

	return EcsComponentListAt(EcsGetComponentList(ecs, type), index);
}

void EcsRemoveOrderComponent(Ecs* ecs, EcsEntityID entity, EcsComponentType type)
{
	if (type >= ecs->order_components.used) return;

	EcsComponentListRemove(EcsGetComponentList(ecs, type), entity);
}

void EcsRemoveEntity(Ecs* ecs, EcsEntityID entity)
{
	for (uint32_t i = 0; i < ecs->data_components.used; ++i)
		EcsRemoveDataComponent(ecs, entity, i);
}
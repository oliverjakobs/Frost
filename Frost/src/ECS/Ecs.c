#include "Ecs.h"

#include <string.h>

void EcsInit(Ecs* ecs)
{
	clib_array_alloc(&ecs->systems_event, ECS_DEFAULT_EVENT_SYSTEM_COUNT, sizeof(EcsEventSystem));
	clib_array_alloc(&ecs->systems_update, ECS_DEFAULT_UPDATE_SYSTEM_COUNT, sizeof(EcsUpdateSystem));
	clib_array_alloc(&ecs->systems_render, ECS_DEFAULT_RENDER_SYSTEM_COUNT, sizeof(EcsRenderSystem));
	clib_array_alloc(&ecs->systems_render_debug, ECS_DEFAULT_RENDER_DEBUG_SYSTEM_COUNT, sizeof(EcsRenderSystem));
	clib_array_alloc(&ecs->systems_render_ui, ECS_DEFAULT_RENDER_UI_SYSTEM_COUNT, sizeof(EcsRenderSystem));

	clib_array_alloc(&ecs->data_components, ECS_DEFAULT_DATA_COMPONENT_COUNT, sizeof(ComponentMap));
	clib_array_alloc(&ecs->order_components, ECS_DEFAULT_ORDER_COMPONENT_COUNT, sizeof(ComponentList));
}

void EcsDestroy(Ecs* ecs)
{
	clib_array_free(&ecs->systems_event);
	clib_array_free(&ecs->systems_update);
	clib_array_free(&ecs->systems_render);
	clib_array_free(&ecs->systems_render_debug);
	clib_array_free(&ecs->systems_render_ui);

	EcsClear(ecs);
	for (size_t i = 0; i < ecs->data_components.used; ++i)
	{
		ComponentMapFree(clib_array_get(&ecs->data_components, i));
	}
	clib_array_free(&ecs->data_components);

	for (size_t i = 0; i < ecs->order_components.used; ++i)
	{
		ComponentListFree(clib_array_get(&ecs->order_components, i));
	}
	clib_array_free(&ecs->order_components);
}

void EcsClear(Ecs* ecs)
{
	for (size_t i = 0; i < ecs->data_components.used; ++i)
	{
		ComponentMapClear(clib_array_get(&ecs->data_components, i));
	}
	for (size_t i = 0; i < ecs->order_components.used; ++i)
	{
		ComponentListClear(clib_array_get(&ecs->order_components, i));
	}
	EntityResetIDCounter();
}

void EcsAddEventSystem(Ecs* ecs, void(*handle)(Ecs*, Event))
{
	EcsEventSystem system;
	system.handle = handle;

	clib_array_push_and_grow(&ecs->systems_event, &system, ECS_ARRAY_GROWTH_FACTOR);
}

void EcsAddUpdateSystem(Ecs* ecs, void(*update)(Ecs*,float))
{
	EcsUpdateSystem system;
	system.update = update;

	clib_array_push_and_grow(&ecs->systems_update, &system, ECS_ARRAY_GROWTH_FACTOR);
}

void EcsAddRenderSystem(Ecs* ecs, void (*render)(Ecs*,const float*))
{
	EcsRenderSystem system;
	system.render = render;

	clib_array_push_and_grow(&ecs->systems_render, &system, ECS_ARRAY_GROWTH_FACTOR);
}

void EcsAddRenderDebugSystem(Ecs* ecs, void(*render)(Ecs*, const float*))
{
	EcsRenderSystem system;
	system.render = render;

	clib_array_push_and_grow(&ecs->systems_render_debug, &system, ECS_ARRAY_GROWTH_FACTOR);
}

void EcsAddRenderUISystem(Ecs* ecs, void(*render)(Ecs*, const float*))
{
	EcsRenderSystem system;
	system.render = render;

	clib_array_push_and_grow(&ecs->systems_render_ui, &system, ECS_ARRAY_GROWTH_FACTOR);
}

void EcsOnEvent(Ecs* ecs, Event e)
{
	for (size_t i = 0; i < ecs->systems_event.used; ++i)
		((EcsEventSystem*)clib_array_get(&ecs->systems_event, i))->handle(ecs, e);
}

void EcsOnUpdate(Ecs* ecs, float deltatime)
{
	for (size_t i = 0; i < ecs->systems_update.used; ++i)
		((EcsUpdateSystem*)clib_array_get(&ecs->systems_update, i))->update(ecs, deltatime);
}

void EcsOnRender(Ecs* ecs, const float* mat_view_proj)
{
	for (size_t i = 0; i < ecs->systems_render.used; ++i)
		((EcsRenderSystem*)clib_array_get(&ecs->systems_render, i))->render(ecs, mat_view_proj);
}

void EcsOnRenderDebug(Ecs* ecs, const float* mat_view_proj)
{
	for (size_t i = 0; i < ecs->systems_render_debug.used; ++i)
		((EcsRenderSystem*)clib_array_get(&ecs->systems_render_debug, i))->render(ecs, mat_view_proj);
}

void EcsOnRenderUI(Ecs* ecs, const float* mat_view_proj)
{
	for (size_t i = 0; i < ecs->systems_render_ui.used; ++i)
		((EcsRenderSystem*)clib_array_get(&ecs->systems_render_ui, i))->render(ecs, mat_view_proj);
}

static void EcsComponentFree(void* block)
{
	free(block);
}

ComponentMap* EcsGetComponentMap(Ecs* ecs, ComponentType type)
{
	return clib_array_get(&ecs->data_components, type);
}

ComponentList* EcsGetComponentList(Ecs* ecs, ComponentType type)
{
	return clib_array_get(&ecs->order_components, type);
}

ComponentType EcsRegisterDataComponent(Ecs* ecs, size_t element_size, void (*free_func)(void*))
{
	ComponentMap comp;
	if (ComponentMapAlloc(&comp, element_size, free_func ? free_func : EcsComponentFree) == 0)
	{
		if (clib_array_push_and_grow(&ecs->data_components, &comp, ECS_ARRAY_GROWTH_FACTOR))
			return ecs->data_components.used - 1;
	}
	return 0;
}

void* EcsAddDataComponent(Ecs* ecs, EntityID entity, ComponentType type, void* component)
{
	ComponentMap* map = EcsGetComponentMap(ecs, type);

	if (!map) return NULL;

	void* data = malloc(map->element_size);
	memcpy(data, component, map->element_size);

	return ComponentMapInsert(map, entity, data);
}

void* EcsGetDataComponent(Ecs* ecs, EntityID entity, ComponentType type)
{
	if (type >= ecs->data_components.used) return NULL;

	return ComponentMapFind(EcsGetComponentMap(ecs, type), entity);
}

void EcsRemoveDataComponent(Ecs* ecs, EntityID entity, ComponentType type)
{
	if (type >= ecs->data_components.used) return;

	ComponentMapRemove(EcsGetComponentMap(ecs, type), entity);
}

ComponentType EcsRegisterOrderComponent(Ecs* ecs, size_t element_size, int (*cmp)(const void*, const void*))
{
	ComponentList comp;
	if (ComponentListAlloc(&comp, element_size, cmp) == 0)
	{
		if (clib_array_push_and_grow(&ecs->order_components, &comp, ECS_ARRAY_GROWTH_FACTOR))
			return ecs->order_components.used - 1;
	}
	return 0;
}

void* EcsAddOrderComponent(Ecs* ecs, ComponentType type, void* component)
{
	ComponentList* list = EcsGetComponentList(ecs, type);

	if (!list) return NULL;

	return ComponentListInsert(list, component);
}

void* EcsGetOrderComponent(Ecs* ecs, size_t index, ComponentType type)
{
	if (type >= ecs->order_components.used) return NULL;

	return ComponentListAt(EcsGetComponentList(ecs, type), index);
}

void EcsRemoveOrderComponent(Ecs* ecs, EntityID entity, ComponentType type)
{
	if (type >= ecs->order_components.used) return;

	ComponentListRemove(EcsGetComponentList(ecs, type), entity);
}

void EcsRemoveEntity(Ecs* ecs, EntityID entity)
{
	for (uint32_t i = 0; i < ecs->data_components.used; ++i)
	{
		EcsRemoveDataComponent(ecs, entity, i);
	}
}
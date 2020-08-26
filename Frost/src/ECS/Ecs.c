#include "Ecs.h"

void EcsInit(Ecs* ecs, size_t update_systems, size_t render_systems, size_t component_count)
{
	clib_array_alloc(&ecs->systems_update, update_systems, sizeof(EcsUpdateSystem));
	clib_array_alloc(&ecs->systems_render, render_systems, sizeof(EcsRenderSystem));

	clib_array_alloc(&ecs->data_components, component_count, sizeof(ComponentMap));
	clib_array_alloc(&ecs->order_components, component_count, sizeof(ComponentList));
}

void EcsDestroy(Ecs* ecs)
{
	clib_array_free(&ecs->systems_update);
	clib_array_free(&ecs->systems_render);

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

void EcsAddUpdateSystem(Ecs* ecs, void(*update)(Ecs*,float))
{
	EcsUpdateSystem system;
	system.update = update;

	clib_array_push(&ecs->systems_update, &system);
}

void EcsAddRenderSystem(Ecs* ecs, void (*render)(Ecs*,const float*))
{
	EcsRenderSystem system;
	system.render = render;

	clib_array_push(&ecs->systems_render, &system);
}

void EcsUpdate(Ecs* ecs, float deltatime)
{
	for (size_t i = 0; i < ecs->systems_update.used; ++i)
		((EcsUpdateSystem*)clib_array_get(&ecs->systems_update, i))->update(ecs, deltatime);
}

void EcsRender(Ecs* ecs, const float* mat_view_proj)
{
	for (size_t i = 0; i < ecs->systems_render.used; ++i)
		((EcsRenderSystem*)clib_array_get(&ecs->systems_render, i))->render(ecs, mat_view_proj);
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
		if (clib_array_push(&ecs->data_components, &comp))
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
		if (clib_array_push(&ecs->order_components, &comp))
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
#include "Ecs.h"

void EcsInit(Ecs* ecs, size_t update_systems, size_t render_systems, size_t component_count)
{
	clib_array_alloc(&ecs->systems_update, update_systems, sizeof(EcsUpdateSystem));
	clib_array_alloc(&ecs->systems_render, render_systems, sizeof(EcsRenderSystem));

	clib_array_alloc(&ecs->table, component_count, sizeof(ComponentMap));
	clib_array_alloc(&ecs->order_components, component_count, sizeof(ComponentList));
}

void EcsDestroy(Ecs* ecs)
{
	clib_array_free(&ecs->systems_update);
	clib_array_free(&ecs->systems_render);

	EcsClearComponents(ecs);
	for (size_t i = 0; i < ecs->table.used; ++i)
	{
		ComponentMapFree(clib_array_get(&ecs->table, i));
	}
	clib_array_free(&ecs->table);

	for (size_t i = 0; i < ecs->order_components.used; ++i)
	{
		ComponentListFree(clib_array_get(&ecs->order_components, i));
	}
	clib_array_free(&ecs->order_components);
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
	return clib_array_get(&ecs->table, type);
}

ComponentList* EcsGetComponentList(Ecs* ecs, ComponentType type)
{
	return clib_array_get(&ecs->order_components, type);
}

void EcsClearComponents(Ecs* ecs)
{
	for (size_t i = 0; i < ecs->table.used; ++i)
	{
		ComponentMapClear(clib_array_get(&ecs->table, i));
	}
}

ComponentType EcsRegisterDataComponent(Ecs* ecs, size_t element_size, void (*free_func)(void*))
{
	ComponentMap comp;
	if (ComponentMapAlloc(&comp, element_size, free_func ? free_func : EcsComponentFree) == 0)
	{
		if (clib_array_push(&ecs->table, &comp))
			return ecs->table.used - 1;
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
	if (type >= ecs->table.used) return NULL;

	return ComponentMapFind(EcsGetComponentMap(ecs, type), entity);
}

void EcsRemoveDataComponent(Ecs* ecs, EntityID entity, ComponentType type)
{
	if (type >= ecs->table.used) return;

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
	for (uint32_t i = 0; i < ecs->table.used; ++i)
	{
		EcsRemoveDataComponent(ecs, entity, i);
	}
}

EntityID EcsGetEntityAt(Ecs* ecs, vec2 pos)
{
	CLIB_HASHMAP_ITERATE_FOR(clib_array_get(&ecs->table, COMPONENT_TRANSFORM), iter)
	{
		Transform* transform = clib_hashmap_iter_get_value(iter);

		vec2 min = vec2_sub(transform->position, (vec2) { transform->size.x / 2.0f, 0.0f });
		vec2 max = vec2_add(min, transform->size);

		if (vec2_inside(pos, min, max))
			return *(EntityID*)clib_hashmap_iter_get_key(iter);
	}

	return NULL_ENTITY;
}

void EcsSetEntityPosition(Ecs* ecs, EntityID entity, vec2 pos)
{
	Transform* transform = EcsGetDataComponent(ecs, entity, COMPONENT_TRANSFORM);
	if (transform) transform->position = pos;

	RigidBody* body = EcsGetDataComponent(ecs, entity, COMPONENT_RIGID_BODY);
	if (body) body->position = vec2_add(pos, body->offset);
}

vec2 EcsGetEntityPosition(Ecs* ecs, EntityID entity)
{
	Transform* transform = EcsGetDataComponent(ecs, entity, COMPONENT_TRANSFORM);
	if (transform) return transform->position;

	return vec2_zero();
}

vec2 EcsGetEntityCenter(Ecs* ecs, EntityID entity)
{
	Transform* transform = EcsGetDataComponent(ecs, entity, COMPONENT_TRANSFORM);

	if (transform)
	{
		vec2 center;
		center.x = transform->position.x;
		center.y = transform->position.y + (transform->size.y / 2.0f);

		return center;
	}

	return vec2_zero();
}

rect EcsGetEntityRect(Ecs* ecs, EntityID entity)
{
	Transform* transform = EcsGetDataComponent(ecs, entity, COMPONENT_TRANSFORM);

	rect r;
	r.min = vec2_zero();
	r.max = vec2_zero();

	if (transform)
	{
		r.min = vec2_sub(transform->position, (vec2) { transform->size.x / 2.0f, 0.0f });
		r.max = vec2_add(r.min, transform->size);
	}

	return r;
}
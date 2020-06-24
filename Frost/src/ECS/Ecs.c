#include "Ecs.h"

void EcsInit(Ecs* ecs, size_t update_systems, size_t render_systems)
{
	clib_array_alloc(&ecs->systems_update, update_systems, sizeof(EcsUpdateSystem));
	clib_array_alloc(&ecs->systems_render, render_systems, sizeof(EcsRenderSystem));

	clib_array_alloc(&ecs->indexed_entities, 16, sizeof(ZIndexedEntity));
}

void EcsDestroy(Ecs* ecs)
{
	clib_array_free(&ecs->systems_update);
	clib_array_free(&ecs->systems_render);

	clib_array_free(&ecs->indexed_entities);
}

void EcsRemoveEntity(Ecs* ecs, const char* entity)
{
	/* TODO */
}

void EcsClearEntities(Ecs* ecs)
{
	clib_array_clear(&ecs->indexed_entities);
}

void EcsAddUpdateSystem(Ecs* ecs, void(*update)(Ecs*,ComponentTable*,float))
{
	EcsUpdateSystem system;
	system.update = update;

	clib_array_push(&ecs->systems_update, &system);
}

void EcsAddRenderSystem(Ecs* ecs, void (*render)(Ecs*,ComponentTable*,const float*))
{
	EcsRenderSystem system;
	system.render = render;

	clib_array_push(&ecs->systems_render, &system);
}

void EcsUpdate(Ecs* ecs, ComponentTable* components, float deltatime)
{
	for (size_t i = 0; i < ecs->systems_update.used; ++i)
		((EcsUpdateSystem*)clib_array_get(&ecs->systems_update, i))->update(ecs, components, deltatime);
}

void EcsRender(Ecs* ecs, ComponentTable* components, const float* mat_view_proj)
{
	for (size_t i = 0; i < ecs->systems_render.used; ++i)
		((EcsRenderSystem*)clib_array_get(&ecs->systems_render, i))->render(ecs, components, mat_view_proj);
}

static int _EcsIndexedEntityCmp(const ZIndexedEntity* a, const ZIndexedEntity* b)
{
	return a->z_index - b->z_index;
}

void EcsAddIndexedEntity(Ecs* ecs, const char* entity, int z_index)
{
	ZIndexedEntity indexed;
	indexed.z_index = z_index;
	indexed.entity = entity;

	clib_array_push(&ecs->indexed_entities, &indexed);
	clib_array_sort(&ecs->indexed_entities, _EcsIndexedEntityCmp);
}

int EcsGetEntityIndex(Ecs* ecs, const char* entity)
{
	for (size_t i = 0; i < ecs->indexed_entities.used; ++i)
	{
		ZIndexedEntity* indexed = clib_array_get(&ecs->indexed_entities, i);

		if (strcmp(indexed->entity, entity) == 0)
			return indexed->z_index;
	}

	return 0;
}

void EntitySetPosition(const char* entity, ComponentTable* components, vec2 pos)
{
	Transform* transform = ComponentTableGetComponent(components, entity, COMPONENT_TRANSFORM);
	if (transform) transform->position = pos;

	RigidBody* body = ComponentTableGetComponent(components, entity, COMPONENT_RIGID_BODY);
	if (body) body->position = vec2_add(pos, body->offset);
}

vec2 EntityGetPosition(const char* entity, ComponentTable* components)
{
	Transform* transform = ComponentTableGetComponent(components, entity, COMPONENT_TRANSFORM);
	if (transform) return transform->position;

	return vec2_zero();
}

vec2 EntityGetCenter(const char* entity, ComponentTable* components)
{
	Transform* transform = ComponentTableGetComponent(components, entity, COMPONENT_TRANSFORM);

	if (transform)
	{
		vec2 center;
		center.x = transform->position.x;
		center.y = transform->position.y + (transform->size.y / 2.0f);

		return center;
	}

	return vec2_zero();
}

rect EntityGetRect(const char* entity, ComponentTable* components)
{
	Transform* transform = ComponentTableGetComponent(components, entity, COMPONENT_TRANSFORM);

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
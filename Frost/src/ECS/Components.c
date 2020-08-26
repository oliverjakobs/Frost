#include "Components.h"

EntityID EcsGetEntityAt(Ecs* ecs, vec2 pos)
{
	CLIB_HASHMAP_ITERATE_FOR(clib_array_get(&ecs->data_components, COMPONENT_TRANSFORM), iter)
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

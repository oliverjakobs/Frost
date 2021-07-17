#include "Transform.h"

#include "Frost/FrostParser.h"

void TransformLoad(char* ini, Ecs* ecs, EcsEntityID entity, vec2 pos)
{
	tb_ini_element element;
	tb_ini_query(ini, "transform", NULL, &element);
	if (element.error == TB_INI_OK)
	{
		Transform transform;
		transform.position = pos;

		transform.size.x = tb_ini_query_float(element.start, NULL, "width", 0.0f);
		transform.size.y = tb_ini_query_float(element.start, NULL, "height", 0.0f);

		EcsAddDataComponent(ecs, entity, COMPONENT_TRANSFORM, &transform);
	}
}

EcsEntityID GetEntityAt(const Ecs* ecs, vec2 pos)
{
	EcsMap* map = EcsGetComponentMap(ecs, COMPONENT_TRANSFORM);
	for (EcsMapIter* iter = EcsMapIterator(map); iter; iter = EcsMapIterNext(map, iter))
	{
		Transform* transform = EcsMapIterValue(iter);

		vec2 min = vec2_sub(transform->position, (vec2) { transform->size.x / 2.0f, 0.0f });
		vec2 max = vec2_add(min, transform->size);

		if (vec2_inside(pos, min, max))
			return EcsMapIterKey(iter);
	}

	return ECS_NULL_ENTITY;
}

void SetEntityPosition(const Ecs* ecs, EcsEntityID entity, vec2 pos)
{
	Transform* transform = EcsGetDataComponent(ecs, entity, COMPONENT_TRANSFORM);
	if (transform) transform->position = pos;

	RigidBody* body = EcsGetDataComponent(ecs, entity, COMPONENT_RIGID_BODY);
	if (body) body->body.position = vec2_add(pos, body->offset);
}

vec2 GetEntityPosition(const Ecs* ecs, EcsEntityID entity)
{
	Transform* transform = EcsGetDataComponent(ecs, entity, COMPONENT_TRANSFORM);
	if (transform) return transform->position;

	return vec2_zero();
}

vec2 GetEntityCenter(const Ecs* ecs, EcsEntityID entity)
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

rect GetEntityRect(const Ecs* ecs, EcsEntityID entity)
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
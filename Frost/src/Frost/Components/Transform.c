#include "Transform.h"

#include "Frost\FrostEcs.h"

#include "toolbox/tb_json.h"

void TransformLoad(Scene* scene, EcsEntityID entity, vec2 pos, char* json)
{
	tb_json_element element;
	tb_json_read(json, &element, "{'transform'");
	if (element.error == TB_JSON_OK)
	{
		Transform transform;
		transform.position = pos;

		transform.size.x = tb_json_float(element.value, "{'size'[0", NULL, 0.0f);
		transform.size.y = tb_json_float(element.value, "{'size'[1", NULL, 0.0f);

		EcsAddDataComponent(&scene->ecs, entity, COMPONENT_TRANSFORM, &transform);
	}
}

EcsEntityID GetEntityAt(Ecs* ecs, vec2 pos)
{
	EcsComponentMap* map = EcsGetComponentMap(ecs, COMPONENT_TRANSFORM);
	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		Transform* transform = EcsComponentMapIterValue(iter);

		vec2 min = vec2_sub(transform->position, (vec2) { transform->size.x / 2.0f, 0.0f });
		vec2 max = vec2_add(min, transform->size);

		if (vec2_inside(pos, min, max))
			return EcsComponentMapIterKey(iter);
	}

	return ECS_NULL_ENTITY;
}

void SetEntityPosition(Ecs* ecs, EcsEntityID entity, vec2 pos)
{
	Transform* transform = EcsGetDataComponent(ecs, entity, COMPONENT_TRANSFORM);
	if (transform) transform->position = pos;

	RigidBody* body = EcsGetDataComponent(ecs, entity, COMPONENT_RIGID_BODY);
	if (body) body->position = vec2_add(pos, body->offset);
}

vec2 GetEntityPosition(Ecs* ecs, EcsEntityID entity)
{
	Transform* transform = EcsGetDataComponent(ecs, entity, COMPONENT_TRANSFORM);
	if (transform) return transform->position;

	return vec2_zero();
}

vec2 GetEntityCenter(Ecs* ecs, EcsEntityID entity)
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

rect GetEntityRect(Ecs* ecs, EcsEntityID entity)
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
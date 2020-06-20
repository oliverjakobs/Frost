#include "PhysicsSystem.h"

void EcsSystemPhysics(ComponentTable* components, const char* entity, float deltatime)
{
	Transform* transform = ComponentTableGetComponent(components, entity, COMPONENT_TRANSFORM);
	RigidBody* body = ComponentTableGetComponent(components, entity, COMPONENT_RIGID_BODY);

	if (!(transform && body)) return;

	body->position = vec2_add(transform->position, body->offset);

	if (body->type == RIGID_BODY_STATIC) return;

	vec2 gravity = (vec2){ 0.0f, -980.0f };

	vec2 old_position = body->position;
	RigidBodyTick(body, gravity, deltatime);

	CLIB_DICT_ITERATE_FOR(&components->components[COMPONENT_RIGID_BODY], iter)
	{
		if (strcmp(entity, clib_dict_iter_get_key(iter)) == 0)
			continue;

		RigidBody* other = clib_dict_iter_get_value(iter);
		RigidBodyResolveCollision(body, other, old_position);
	}

	transform->position = vec2_sub(body->position, body->offset);
}

#include "PhysicsSystem.h"

void EcsSystemPhysics(ComponentTable* components, float deltatime)
{
	CLIB_DICT_ITERATE_FOR(&components->table[COMPONENT_RIGID_BODY], iter)
	{
		RigidBody* body = clib_dict_iter_get_value(iter);

		Transform* transform = ComponentTableGetComponent(components, clib_dict_iter_get_key(iter), COMPONENT_TRANSFORM);
		if (!transform) continue;

		body->position = vec2_add(transform->position, body->offset);

		if (body->type == RIGID_BODY_STATIC) continue;

		vec2 gravity = (vec2){ 0.0f, -980.0f };

		vec2 old_position = body->position;
		RigidBodyTick(body, gravity, deltatime);

		CLIB_DICT_ITERATE_FOR(&components->table[COMPONENT_RIGID_BODY], other_iter)
		{
			if (strcmp(clib_dict_iter_get_key(iter), clib_dict_iter_get_key(other_iter)) == 0)
				continue;

			RigidBody* other = clib_dict_iter_get_value(other_iter);
			RigidBodyResolveCollision(body, other, old_position);
		}

		transform->position = vec2_sub(body->position, body->offset);
	}
}

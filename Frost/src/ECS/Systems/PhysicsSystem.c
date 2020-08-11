#include "PhysicsSystem.h"

void PhysicsSystem(Ecs* ecs, ComponentTable* components, float deltatime)
{
	COMPONENT_MAP_ITERATE_FOR(clib_array_get(&components->table, COMPONENT_RIGID_BODY), iter)
	{
		RigidBody* body = ComponentMapIterValue(iter);

		Transform* transform = ComponentTableGetDataComponent(components, ComponentMapIterKey(iter), COMPONENT_TRANSFORM);
		if (!transform) continue;

		body->position = vec2_add(transform->position, body->offset);

		if (body->type == RIGID_BODY_STATIC) continue;

		vec2 gravity = (vec2){ 0.0f, -980.0f };

		vec2 old_position = body->position;
		RigidBodyTick(body, gravity, deltatime);

		COMPONENT_MAP_ITERATE_FOR(clib_array_get(&components->table, COMPONENT_RIGID_BODY), other_iter)
		{
			if (ComponentMapIterKey(iter) == ComponentMapIterKey(other_iter))
				continue;

			RigidBody* other = ComponentMapIterValue(other_iter);
			RigidBodyResolveCollision(body, other, old_position);
		}

		transform->position = vec2_sub(body->position, body->offset);
	}
}

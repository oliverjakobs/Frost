#include "PhysicsSystem.h"

#include "Frost/FrostEcs.h"

void PhysicsSystem(Ecs* ecs, float deltatime)
{
	EcsComponentMap* map = EcsGetComponentMap(ecs, COMPONENT_RIGID_BODY);
	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		RigidBody* body = EcsComponentMapIterValue(iter);

		Transform* transform = EcsGetDataComponent(ecs, EcsComponentMapIterKey(iter), COMPONENT_TRANSFORM);
		if (!transform) continue;

		body->position = vec2_add(transform->position, body->offset);

		if (body->type == RIGID_BODY_STATIC) continue;

		vec2 gravity = (vec2){ 0.0f, -980.0f };

		vec2 old_position = body->position;
		RigidBodyTick(body, gravity, deltatime);

		for (EcsComponentMapIter* other_iter = EcsComponentMapIterator(map); other_iter; other_iter = EcsComponentMapIterNext(map, other_iter))
		{
			if (EcsComponentMapIterKey(iter) == EcsComponentMapIterKey(other_iter)) continue;

			RigidBody* other = EcsComponentMapIterValue(other_iter);
			RigidBodyResolveCollision(body, other, old_position);
		}

		transform->position = vec2_sub(body->position, body->offset);
	}
}

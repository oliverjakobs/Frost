#include "PhysicsSystem.h"

#include "Frost/FrostEcs.h"

#include "tile/tile_collision.h"

static const float SLOPE_GRIP = 100.0f;

void PhysicsSystem(Ecs* ecs, const Scene* scene, float deltatime)
{
	EcsMap* map = EcsGetComponentMap(ecs, COMPONENT_RIGID_BODY);
	for (EcsMapIter* iter = EcsMapIterator(map); iter; iter = EcsMapIterNext(map, iter))
	{
		RigidBody* body = EcsMapIterValue(iter);

		Transform* transform = EcsGetDataComponent(ecs, EcsMapIterKey(iter), COMPONENT_TRANSFORM);
		if (!transform) continue;

		body->body.position = vec2_add(transform->position, body->offset);

		if (body->body.type == TILE_BODY_STATIC) continue;

		vec2 old_position = body->body.position;

		TileBodyApplyGravity(&body->body, scene->gravity, SLOPE_GRIP, deltatime);
		TileBodyTick(&body->body, &scene->map, deltatime);

		for (EcsMapIter* other_iter = EcsMapIterator(map); other_iter; other_iter = EcsMapIterNext(map, other_iter))
		{
			if (EcsMapIterKey(iter) == EcsMapIterKey(other_iter)) continue;

			RigidBody* other = EcsMapIterValue(other_iter);
			if (body->filter & other->filter)
				TileResolveBodies(&body->body, &other->body, old_position);
		}

		transform->position = vec2_sub(body->body.position, body->offset);
	}
}

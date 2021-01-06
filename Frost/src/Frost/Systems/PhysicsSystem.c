#include "PhysicsSystem.h"

#include "Frost/FrostEcs.h"

#include "tile/collision.h"

static const vec2 GRAVITY = { 0.0f, -980.0f };
static const float SLOPE_GRIP = 100.0f;

void PhysicsSystem(Ecs* ecs, float deltatime)
{
	EcsComponentMap* map = EcsGetComponentMap(ecs, COMPONENT_RIGID_BODY);
	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		RigidBody* body = EcsComponentMapIterValue(iter);

		Transform* transform = EcsGetDataComponent(ecs, EcsComponentMapIterKey(iter), COMPONENT_TRANSFORM);
		if (!transform) continue;

		body->body.position = vec2_add(transform->position, body->offset);

		if (body->body.type == TILE_BODY_STATIC) continue;

		vec2 old_position = body->body.position;

		TileBodyApplyGravity(&body->body, GRAVITY, SLOPE_GRIP, deltatime);
		TileBodyTick(&body->body, deltatime);

		for (EcsComponentMapIter* other_iter = EcsComponentMapIterator(map); other_iter; other_iter = EcsComponentMapIterNext(map, other_iter))
		{
			if (EcsComponentMapIterKey(iter) == EcsComponentMapIterKey(other_iter)) continue;

			RigidBody* other = EcsComponentMapIterValue(other_iter);
			if (body->filter & other->filter)
				TileResolveBodies(&body->body, &other->body, old_position);
		}

		transform->position = vec2_sub(body->body.position, body->offset);
	}
}

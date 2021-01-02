#include "EntityStateSystem.h"

#include "Frost/FrostEcs.h"

int EntityStateJump(RigidBody* body) { return body ? (body->body.velocity.y > 0.0f) : 0; }
int EntityStateFall(RigidBody* body) { return body ? (!body->body.collision_state[TILE_BOTTOM] && (body->body.velocity.y <= 0.0f)) : 0; }
int EntityStateWalk(RigidBody* body) { return body ? (body->body.collision_state[TILE_BOTTOM] && (body->body.velocity.x != 0.0f)) : 0; }
int EntityStateIdle(RigidBody* body) { return body ? (body->body.collision_state[TILE_BOTTOM] && (body->body.velocity.x == 0.0f)) : 0; }

void EntityStateSystem(Ecs* ecs, float deltatime)
{
	EcsComponentMap* map = EcsGetComponentMap(ecs, COMPONENT_STATE);
	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		EntityState* state = EcsComponentMapIterValue(iter);
		ECS_COMPONENT_OPTIONAL(RigidBody, ecs, body, iter, COMPONENT_RIGID_BODY);

		if (EntityStateJump(body)) *state = ENTITY_STATE_JUMP;
		else if (EntityStateFall(body)) *state = ENTITY_STATE_FALL;
		else if (EntityStateWalk(body)) *state = ENTITY_STATE_WALK;
		else if (EntityStateIdle(body)) *state = ENTITY_STATE_IDLE;
		else *state = ENTITY_STATE_JUMP;
	}
}
#include "EntityStateSystem.h"

#include "Frost/Frost.h"

int EntityStateJump(RigidBody* body) { return body ? (body->body.velocity.y > 0.0f) : 0; }
int EntityStateFall(RigidBody* body) { return body ? (!body->body.collision_state[TILE_BOTTOM] && (body->body.velocity.y <= 0.0f)) : 0; }
int EntityStateWalk(RigidBody* body) { return body ? (body->body.collision_state[TILE_BOTTOM] && (body->body.velocity.x != 0.0f)) : 0; }
int EntityStateIdle(RigidBody* body) { return body ? (body->body.collision_state[TILE_BOTTOM] && (body->body.velocity.x == 0.0f)) : 0; }

void EntityStateSystem(Ecs* ecs, const Scene* scene, float deltatime)
{
	EcsMap* map = EcsGetComponentMap(ecs, COMPONENT_STATE);
	for (EcsMapIter* iter = EcsMapIterator(map); iter; iter = EcsMapIterNext(map, iter))
	{
		EntityState* state = EcsMapIterValue(iter);
		ECS_OPTIONAL_MAP(RigidBody, ecs, body, iter, COMPONENT_RIGID_BODY);

		if (EntityStateJump(body)) *state = ENTITY_STATE_JUMP;
		else if (EntityStateFall(body)) *state = ENTITY_STATE_FALL;
		else if (EntityStateWalk(body)) *state = ENTITY_STATE_WALK;
		else if (EntityStateIdle(body)) *state = ENTITY_STATE_IDLE;
		else *state = ENTITY_STATE_JUMP;
	}
}
#include "AnimationConditions.h"

#include "Frost/FrostEcs.h"

int AnimationConditionJump(Ecs* ecs, EntityID entity, int s)
{
	RigidBody* body = EcsGetDataComponent(ecs, entity, COMPONENT_RIGID_BODY);

	if (body) return body->velocity.y > 0.0f;

	return 0;
}

int AnimationConditionFall(Ecs* ecs, EntityID entity, int s)
{
	RigidBody* body = EcsGetDataComponent(ecs, entity, COMPONENT_RIGID_BODY);

	if (body) return !body->collides_bottom && body->velocity.y <= 0.0f;

	return 0;
}

int AnimationConditionWalk(Ecs* ecs, EntityID entity, int s)
{
	RigidBody* body = EcsGetDataComponent(ecs, entity, COMPONENT_RIGID_BODY);

	if (body) return body->collides_bottom && body->velocity.x != 0.0f;

	return 0;
}

int AnimationConditionIdle(Ecs* ecs, EntityID entity, int s)
{
	RigidBody* body = EcsGetDataComponent(ecs, entity, COMPONENT_RIGID_BODY);

	if (body) return body->collides_bottom && body->velocity.x == 0.0f;

	return 0;
}

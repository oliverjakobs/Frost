#include "AnimationConditions.h"

int AnimationConditionJump(EcsEntity* e, int s)
{
	EcsPhysicsComponent* comp = e->physics;

	if (comp)
		return comp->body->velocity.y > 0.0f;

	return 0;
}

int AnimationConditionFall(EcsEntity* e, int s)
{
	EcsPhysicsComponent* comp = e->physics;

	if (comp)
		return !comp->body->collidesBottom && comp->body->velocity.y <= 0.0f;

	return 0;
}

int AnimationConditionWalk(EcsEntity* e, int s)
{
	EcsPhysicsComponent* comp = e->physics;

	if (comp)
		return comp->body->collidesBottom && comp->body->velocity.x != 0.0f;

	return 0;
}

int AnimationConditionIdle(EcsEntity* e, int s)
{
	EcsPhysicsComponent* comp = e->physics;

	if (comp)
		return comp->body->collidesBottom && comp->body->velocity.x == 0.0f;

	return 0;
}

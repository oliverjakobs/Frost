#include "AnimationConditions.h"

bool AnimationConditionJump(Entity* e, int s)
{
	if (auto comp = EntityGetComponent<PhysicsComponent>(e))
		return comp->GetBody()->velocity.y > 0.0f;

	return false;
}

bool AnimationConditionFall(Entity* e, int s)
{
	if (auto comp = EntityGetComponent<PhysicsComponent>(e))
		return !comp->GetBody()->collidesBottom && comp->GetBody()->velocity.y <= 0.0f;

	return false;
}

bool AnimationConditionWalk(Entity* e, int s)
{
	if (auto comp = EntityGetComponent<PhysicsComponent>(e))
		return comp->GetBody()->collidesBottom && comp->GetBody()->velocity.x != 0.0f;

	return false;
}

bool AnimationConditionIdle(Entity* e, int s)
{
	if (auto comp = EntityGetComponent<PhysicsComponent>(e))
		return comp->GetBody()->collidesBottom && comp->GetBody()->velocity.x == 0.0f;

	return false;
}

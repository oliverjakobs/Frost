#include "AnimationConditions.h"

#include "ECS/Components.h"

int AnimationConditionJump(ComponentTable* components, const char* entity, int s)
{
	EcsPhysicsComponent* physics = ComponentTableGetComponent(components, entity, COMPONENT_PHYSICS);

	if (physics)
		return physics->body.velocity.y > 0.0f;

	return 0;
}

int AnimationConditionFall(ComponentTable* components, const char* entity, int s)
{
	EcsPhysicsComponent* physics = ComponentTableGetComponent(components, entity, COMPONENT_PHYSICS);

	if (physics)
		return !physics->body.collidesBottom && physics->body.velocity.y <= 0.0f;

	return 0;
}

int AnimationConditionWalk(ComponentTable* components, const char* entity, int s)
{
	EcsPhysicsComponent* physics = ComponentTableGetComponent(components, entity, COMPONENT_PHYSICS);

	if (physics)
		return physics->body.collidesBottom && physics->body.velocity.x != 0.0f;

	return 0;
}

int AnimationConditionIdle(ComponentTable* components, const char* entity, int s)
{
	EcsPhysicsComponent* physics = ComponentTableGetComponent(components, entity, COMPONENT_PHYSICS);

	if (physics)
		return physics->body.collidesBottom && physics->body.velocity.x == 0.0f;

	return 0;
}

#include "AnimationConditions.h"

#include "ECS/Components.h"

int AnimationConditionJump(ComponentTable* components, const char* entity, int s)
{
	RigidBody* body = ComponentTableGetComponent(components, entity, COMPONENT_RIGID_BODY);

	if (body)
		return body->velocity.y > 0.0f;

	return 0;
}

int AnimationConditionFall(ComponentTable* components, const char* entity, int s)
{
	RigidBody* body = ComponentTableGetComponent(components, entity, COMPONENT_RIGID_BODY);

	if (body)
		return !body->collides_bottom && body->velocity.y <= 0.0f;

	return 0;
}

int AnimationConditionWalk(ComponentTable* components, const char* entity, int s)
{
	RigidBody* body = ComponentTableGetComponent(components, entity, COMPONENT_RIGID_BODY);

	if (body)
		return body->collides_bottom && body->velocity.x != 0.0f;

	return 0;
}

int AnimationConditionIdle(ComponentTable* components, const char* entity, int s)
{
	RigidBody* body = ComponentTableGetComponent(components, entity, COMPONENT_RIGID_BODY);

	if (body)
		return body->collides_bottom && body->velocity.x == 0.0f;

	return 0;
}

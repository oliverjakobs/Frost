#include "PhysicsSystem.h"

void EcsSystemPhysics(ComponentTable* components, const char* entity, float deltatime)
{
	EcsPhysicsComponent* physics = ComponentTableGetComponent(components, entity, COMPONENT_PHYSICS);
	Transform* transform = ComponentTableGetComponent(components, entity, COMPONENT_TRANSFORM);

	if (!(physics && transform)) return;

	transform->x = physics->body.position.x - physics->body_x;
	transform->y = physics->body.position.y - physics->body_y;
}

#include "PhysicsSystem.h"

void TilePhysicsSystem::Tick(entt::registry& registry)
{
	registry.view<TransformComponent, PhysicsComponent>().each([](auto entity, auto& trans, auto& phys)
	{
		trans.position = phys.body->GetPosition() - phys.bodyPos;
	});
}

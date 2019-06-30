#pragma once

#include "ecsSystem.h"

struct TilePhysicsSystem : ecsSystem
{
	void Tick(entt::registry& registry) override;
};
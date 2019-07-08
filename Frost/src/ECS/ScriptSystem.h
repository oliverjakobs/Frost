#pragma once

#include "ecsSystem.h"

struct ScriptSystem : ecsSystem
{
	void Tick(entt::registry& registry) override;
};
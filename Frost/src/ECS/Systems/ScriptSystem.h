#pragma once

#include "ECS/Components.h"

struct ScriptSystem
{
	static void Tick(entt::registry& registry);
};
#pragma once

#include "Script/Lua/LuaBinding.h"
#include "ecsSystem.h"

struct ScriptSystem : ecsSystem
{
private:
	sol::state lua;

public:
	ScriptSystem(entt::registry& registry);

	void Tick(entt::registry& registry) override;
};
#pragma once

#include "Script/Lua/LuaBinding.h"
#include <entt/entt.hpp>

struct ScriptSystem
{
private:
	sol::state lua;

public:
	ScriptSystem(entt::registry& registry);

	void Tick(entt::registry& registry);
};
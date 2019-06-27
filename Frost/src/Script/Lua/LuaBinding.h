#pragma once

#include <sol/sol.hpp>
#include <entt/entt.hpp>

#include "LuaComponents.h"

class LuaInput
{
private:
	static unsigned int GetKeyCode(const std::string& key);
public:
	static bool KeyPressed(const std::string& key);
	static bool KeyReleased(const std::string& key);
};
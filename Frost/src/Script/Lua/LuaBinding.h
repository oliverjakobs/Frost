#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

class LuaInput
{
private:
	static unsigned int GetKeyCode(const std::string& key);
public:
	static bool KeyPressed(const std::string& key);
	static bool KeyReleased(const std::string& key);
};
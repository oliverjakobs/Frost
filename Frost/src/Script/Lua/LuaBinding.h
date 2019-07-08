#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <entt/entt.hpp>

class LuaInput
{
private:
	static unsigned int GetKeyCode(const std::string& key);
public:
	static bool KeyPressed(const std::string& key);
	static bool KeyReleased(const std::string& key);
};

class LuaBinding
{
private:
	sol::state m_lua;

public:
	void LoadState(entt::registry& registry);

	sol::function BindLuaFunction(const std::string& src, const std::string& func);

	sol::state& GetState();
};

sol::function BindLuaFunction(sol::state& lua, const std::string& src, const std::string& func);
#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "Types.h"

class LuaInput
{
private:
	static unsigned int GetKeyCode(const std::string& key);
public:
	static bool KeyPressed(const std::string& key);
	static bool KeyReleased(const std::string& key);
};

class Entity;
class LuaEntity
{
private:
	Entity* m_entity;

public:
	LuaEntity(Entity* entity);

	glm::vec2 GetVelocity();
	bool CollidesBottom();

	void Drop();
	void SetVelocity(const glm::vec2& vel);
	void PlayAnimation(const std::string& animation, int flip);
	void SetView();
	void SetDirection(const std::string& direction);
};

class LuaBinding
{
private:
	sol::state m_lua;

public:
	void LoadState();

	sol::function BindLuaFunction(const std::string& src, const std::string& func);

	sol::state& GetState();
};
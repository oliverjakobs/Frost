#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "LuaScene.h"
#include "LuaEntity.h"

class LuaInput
{
private:
	static int GetKeyCode(const std::string& key);
	static int GetButtonCode(const std::string& button);

public:
	static bool KeyPressed(const std::string& key);
	static bool KeyReleased(const std::string& key);

	static bool MousePressed(const std::string& button);
	static bool MouseReleased(const std::string& button);

	static glm::vec2 MousePosition();
	static float MouseX();
	static float MouseY();
};

void LoadLuaState(sol::state& lua);
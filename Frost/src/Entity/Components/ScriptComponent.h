#pragma once

#include "Component.h"
#include "Script/Lua/LuaBinding.h"

class ScriptComponent: public Component
{
private:
	sol::function m_update;

public:
	ScriptComponent(sol::state& lua, const std::string& script);

	void OnUpdate() override;
	void OnRender() override;
};
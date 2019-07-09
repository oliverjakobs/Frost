#pragma once

#include "Component.h"
#include "Script/Lua/LuaBinding.h"

class ScriptComponent: public Component
{
private:
	sol::function m_update;

public:
	ScriptComponent(sol::function& update);

	void OnUpdate() override;
	void OnRender() override;
};
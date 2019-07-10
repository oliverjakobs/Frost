#include "ScriptComponent.h"

ScriptComponent::ScriptComponent(sol::state& lua, const std::string& script)
{
	lua.script_file(script);

	m_update = lua["onUpdate"];
}

void ScriptComponent::OnUpdate()
{
	m_update(LuaEntity(m_entity));
}

void ScriptComponent::OnRender()
{
}

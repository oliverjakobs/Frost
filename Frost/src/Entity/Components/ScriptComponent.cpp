#include "ScriptComponent.h"

ScriptComponent::ScriptComponent(sol::function& update)
	: m_update(update) 
{ 

}

void ScriptComponent::OnUpdate()
{
	m_update(LuaEntity(m_entity));
}

void ScriptComponent::OnRender()
{
}

#include "Entity.h"

#include "Component.h"

Entity::Entity(const Entity& copy)
{
	m_name = copy.m_name;

	for (auto& c : copy.m_components)
	{
		AddComponent(c->clone());
	}
}

Entity::Entity(const std::string& name) : m_name(name) { }

Entity::~Entity()
{
}

void Entity::OnUpdate()
{
	for (auto& c : m_components)
	{
		c->OnUpdate();
	}
}

void Entity::OnRender()
{
	for (auto& c : m_components)
	{
		c->OnRender();
	}
}

void Entity::OnRenderDebug()
{
	for (auto& c : m_components)
	{
		c->OnRenderDebug();
	}
}

std::string Entity::GetName() const
{
	return m_name;
}

void Entity::AddComponent(Component* component)
{
	if (component->SetEntity(this))
		m_components.push_back(unique_ptr<Component>(component));
}

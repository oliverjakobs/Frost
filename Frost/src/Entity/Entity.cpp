#include "Entity.h"

#include "Components/Component.h"

Entity::Entity(const Entity& copy)
	: m_name(copy.m_name), m_position(copy.m_position), m_dimension(copy.m_dimension), m_direction(copy.m_direction)
{
	for (auto& c : copy.m_components)
	{
		//AddComponent(c->clone());
	}
}

Entity::Entity(const std::string& name, const glm::vec2& position, const glm::vec2& dimension)
	: m_name(name), m_position(position), m_dimension(dimension), m_direction(Direction::RIGHT)
{

}

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

void Entity::SetPosition(const glm::vec2& pos)
{
	m_position = pos;
}

void Entity::SetDimension(const glm::vec2& dim)
{
	m_dimension = dim;
}

void Entity::SetDirection(Direction dir)
{
	m_direction = dir;
}

std::string Entity::GetName() const
{
	return m_name;
}

glm::vec2 Entity::GetPosition() const
{
	return m_position;
}

glm::vec2 Entity::GetDimension() const
{
	return m_dimension;
}

Direction Entity::GetDirection() const
{
	return m_direction;
}

void Entity::AddComponent(Component* component)
{
	if (component->SetEntity(this))
		m_components.push_back(unique_ptr<Component>(component));
}

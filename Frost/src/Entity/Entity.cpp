#include "Entity.h"

#include "Components/Component.h"

#include "Obelisk/Obelisk.h"

Entity::Entity(const std::string& name, const glm::vec2& position, const glm::vec2& dimension)
	: m_name(name), m_position(position), m_dimension(dimension), m_direction(Direction::RIGHT)
{

}

Entity::~Entity()
{
}

void Entity::OnUpdate(float deltaTime)
{
	for (auto& c : m_components)
	{
		c->OnUpdate(deltaTime);
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

std::string DirectionToString(Direction dir)
{
	switch (dir)
	{
	case Direction::NONE:	return "NONE";
	case Direction::LEFT:	return "LEFT";
	case Direction::RIGHT:	return "RIGHT";
	case Direction::UP:		return "UP";
	case Direction::DOWN:	return "DOWN";
	}
}

Direction StringToDirection(const std::string& str)
{
	if (obelisk::StringCompare(str, "LEFT"))
		return Direction::LEFT;
	if (obelisk::StringCompare(str, "RIGHT"))
		return Direction::RIGHT;
	if (obelisk::StringCompare(str, "UP"))
		return Direction::UP;
	if (obelisk::StringCompare(str, "DOWN"))
		return Direction::DOWN;

	return Direction::NONE;
}

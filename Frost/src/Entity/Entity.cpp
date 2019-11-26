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

void Entity::OnUpdate(Scene* scene, float deltaTime)
{
	for (auto& c : m_components)
	{
		c->OnUpdate(scene, deltaTime);
	}
}

void Entity::OnRender(Scene* scene)
{
	for (auto& c : m_components)
	{
		c->OnRender(scene);
	}
}

void Entity::OnRenderDebug()
{
	for (auto& c : m_components)
	{
		c->OnRenderDebug();
	}
}

std::string DirectionToString(Direction dir)
{
	switch (dir)
	{
	case Direction::LEFT:	return "LEFT";
	case Direction::RIGHT:	return "RIGHT";
	case Direction::UP:		return "UP";
	case Direction::DOWN:	return "DOWN";
	default: return "NONE";
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

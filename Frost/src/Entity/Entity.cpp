#include "Entity.hpp"

#include "Components.hpp"

#include "Obelisk/Obelisk.hpp"

Entity::Entity(const std::string& name)
	: m_name(name), m_direction(Direction::RIGHT)
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

void Entity::SetPosition(const glm::vec2& pos)
{
	auto comp = GetComponent<PositionComponent>();

	if (comp == nullptr)
	{
		OBELISK_WARN("Setting position for an entity without position component");
		return;
	}

	comp->SetPosition(pos);
}

glm::vec2 Entity::GetPosition() const
{
	auto comp = GetComponent<PositionComponent>();

	if (comp == nullptr)
	{
		OBELISK_WARN("Getting position from an entity without position component");
		return glm::vec2();
	}

	return comp->GetPosition();
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

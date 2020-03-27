#include "Entity.hpp"

#include "Components.hpp"

#include "Obelisk/Obelisk.hpp"

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

Entity::Entity(const std::string& name)
	: name(name), direction(Direction::RIGHT)
{

}

Entity::~Entity()
{

}

void EntityOnUpdate(Entity* entity, Scene* scene, float deltaTime)
{
	for (auto& c : entity->components)
	{
		c->OnUpdate(scene, deltaTime);
	}
}

void EntityOnRender(Entity* entity, Scene* scene)
{
	for (auto& c : entity->components)
	{
		c->OnRender(scene);
	}
}

void EntityOnRenderDebug(Entity* entity)
{
	for (auto& c : entity->components)
	{
		c->OnRenderDebug();
	}
}

void EntitySetPosition(Entity* entity, const glm::vec2& pos)
{
	auto comp = EntityGetComponent<PositionComponent>(entity);

	if (comp == nullptr)
	{
		OBELISK_WARN("Setting position for an entity without position component");
		return;
	}

	comp->SetPosition(pos);
}

glm::vec2 EntityGetPosition(Entity* entity)
{
	auto comp = EntityGetComponent<PositionComponent>(entity);

	if (comp == nullptr)
	{
		OBELISK_WARN("Getting position from an entity without position component");
		return glm::vec2();
	}

	return comp->GetPosition();
}

#include "Entity.hpp"

#include "Components.hpp"

#include "Debugger.h"

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

Direction StringToDirection(const char* str)
{
	if (strcmp(str, "LEFT") == 0)
		return Direction::LEFT;
	if (strcmp(str, "RIGHT") == 0)
		return Direction::RIGHT;
	if (strcmp(str, "UP") == 0)
		return Direction::UP;
	if (strcmp(str, "DOWN") == 0)
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

void EntitySetPosition(Entity* entity, const vec2 pos)
{
	auto comp = EntityGetComponent<PositionComponent>(entity);

	if (comp == nullptr)
	{
		DEBUG_WARN("Setting position for an entity without position component\n");
		return;
	}

	comp->SetPosition(pos);
}

vec2 EntityGetPosition(Entity* entity)
{
	auto comp = EntityGetComponent<PositionComponent>(entity);

	if (comp == nullptr)
	{
		DEBUG_WARN("Getting position from an entity without position component\n");
		return vec2_(0.0f, 0.0f);
	}

	return comp->GetPosition();
}

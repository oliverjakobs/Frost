#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>

struct Scene;
class Component;
struct EntityManager;

enum class Direction
{
	NONE = -1,
	LEFT = 0, 
	RIGHT = 1, 
	UP = 2, 
	DOWN = 3
};

std::string DirectionToString(Direction dir);
Direction StringToDirection(const std::string& str);

struct Entity
{
	std::string name;
	Direction direction;

	std::vector<std::unique_ptr<Component>> components;

	Entity(const std::string& name);
	~Entity();
};

void EntityOnUpdate(Entity* entity, Scene* scene, float deltaTime);
void EntityOnRender(Entity* entity, Scene* scene);
void EntityOnRenderDebug(Entity* entity);

void EntitySetPosition(Entity* entity, const glm::vec2& pos);
glm::vec2 EntityGetPosition(Entity* entity);

template <class Type, class... Args>
void EntityAddComponent(Entity* entity, Args&&... args)
{
	entity->components.push_back(std::unique_ptr<Type>(new Type(entity, std::forward<Args>(args)...)));
}

template<typename Type>
Type* EntityGetComponent(Entity* entity)
{
	if (entity == nullptr) return nullptr;

	Type* component = nullptr;

	for (auto& c : entity->components)
	{
		component = dynamic_cast<Type*>(c.get());

		if (component != nullptr)
			break;
	}

	return component;
}
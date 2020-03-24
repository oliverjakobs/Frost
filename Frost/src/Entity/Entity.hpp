#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>

class Scene;
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

class Entity
{
private:
	std::string m_name;

	Direction m_direction;

	std::vector<std::unique_ptr<Component>> m_components;
public:
	Entity(const std::string& name);
	~Entity();

	void OnUpdate(Scene* scene, float deltaTime);
	void OnRender(Scene* scene);
	void OnRenderDebug();

	void SetPosition(const glm::vec2& pos);
	glm::vec2 GetPosition() const;

	void SetDirection(Direction dir) { m_direction = dir; }
	Direction GetDirection() const { return m_direction; }

	std::string GetName() const { return m_name; }


	template <class Type, class... Args>
	void AddComponent(Args&&... args)
	{
		m_components.push_back(std::unique_ptr<Type>(new Type(this, std::forward<Args>(args)...)));
	}

	template<typename Type>
	Type* GetComponent() const
	{
		Type* component = nullptr;

		for (auto& c : m_components)
		{
			component = dynamic_cast<Type*>(c.get());

			if (component != nullptr)
				break;
		}

		return component;
	}

	friend EntityManager;
};
#pragma once

#include "Types.h"

class Scene;
class Component;

class Entity
{
private:
	std::string m_name;

	glm::vec2 m_position;
	glm::vec2 m_dimension;

	Direction m_direction;

	std::vector<unique_ptr<Component>> m_components;

	Scene* m_scene;
public:
	Entity(const Entity& copy);
	Entity(const std::string& name, const glm::vec2& position, const glm::vec2& dimension);
	~Entity();

	void OnUpdate();
	void OnRender();
	void OnRenderDebug();

	void SetPosition(const glm::vec2& pos);
	void SetDimension(const glm::vec2& dim);
	void SetDirection(Direction dir);

	std::string GetName() const;

	glm::vec2 GetPosition() const;
	glm::vec2 GetDimension() const;
	Direction GetDirection() const;

	void SetScene(Scene* scene);
	Scene* GetScene() const;

	void AddComponent(Component* component);

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
};
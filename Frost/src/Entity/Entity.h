#pragma once

#include "Types.h"

class Component;

class Entity
{
private:
	std::string m_name;
	std::vector<unique_ptr<Component>> m_components;

public:
	Entity(const Entity& copy);
	Entity(const std::string& name);
	~Entity();

	void OnUpdate();
	void OnRender();
	void OnRenderDebug();

	std::string GetName() const;

	void AddComponent(Component* component);

	template<typename Type>
	Type* GetComponent() const
	{
		Type* component = nullptr;

		for (auto& c : m_components)
		{
			component = dynamic_cast<Type*>(c);

			if (component != nullptr)
				break;
		}

		return component;
	}
};
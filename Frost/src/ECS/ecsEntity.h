#pragma once

#include "ecsTypes.h"
#include "Utility/Debugger.h"

#include <unordered_map>

namespace ecs
{
	class Component;

	class Entity
	{
	private:
		using ComponentMap = std::unordered_map<ComponentID, Component*, std::hash<ComponentID>, std::equal_to<ComponentID>>;

		EntityID m_id;
		ComponentMap m_components;

	public:
		explicit Entity(EntityID id) : m_id(id) {}
		// Entity is not copyable but movable
		Entity(const Entity&) = delete;
		Entity& operator=(const Entity&) = delete;
		Entity(Entity&&) = default;
		Entity& operator=(Entity&&) = default;
		~Entity() = default;

		void addComponent(ComponentID compID, Component* comp)
		{
			DEBUG_ASSERT(m_components.find(compID) == m_components.end(), "Component type already used ({0})", compID);
			m_components.emplace(compID, comp);
		}

		EntityID getID() const { return m_id; }
		const ComponentMap getComponents() const { return m_components; }

		template <class CompType>
		CompType* getComponent() const
		{
			auto findIt = m_components.find(CompType::ID);
			if (findIt != m_components.end())
				return findIt->second;

			return nullptr;
		}
	};
}
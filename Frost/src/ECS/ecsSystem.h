#pragma once

#include "ecsEntity.h"

#include <vector>

namespace ecs
{
	class Registry;

	class BaseSystem
	{
	protected:
		Registry* m_registry;

	public:
		explicit BaseSystem(Registry* reg) : m_registry(reg) {}
		virtual ~BaseSystem() = default;

		virtual void onEntityCreated(const Entity& entity) = 0;
		virtual void onEntityDestroyed(EntityID entityID) = 0;
		virtual void update() = 0;
	};

	template <class... Components>
	class System : public BaseSystem
	{
	private:
		using EntityIDToIndexMap = std::unordered_map<EntityID, size_t, std::hash<EntityID>, std::equal_to<EntityID>>;
		EntityIDToIndexMap m_idToIndexMap;

	protected:
		using ComponentTuple = std::tuple<std::add_pointer_t<Components>...>;
		std::vector<ComponentTuple> m_components;

	public:
		System(Registry* reg) : BaseSystem(reg) {}
		virtual ~System() = default;

		virtual void onEntityCreated(const Entity& entity) override final;
		virtual void onEntityDestroyed(EntityID entityID) override final;

	private:
		// recurssively looks through component types
		template <size_t i, class CompType, class... CompArgs>
		bool processEntityComponent(ComponentID id, Component* component, ComponentTuple& tupleToFill);

		// this i the termination specialization of the recurssion, which happens when we no longer have a 
		// CompType that's passed in
		template <size_t i>
		bool processEntityComponent(ComponentID id, Component* component, ComponentTuple& tupleToFill);

	};

	template<class... Components>
	inline void System<Components...>::onEntityCreated(const Entity& entity)
	{
		// loop through all components on this entity and see if any tuples match
		ComponentTuple compTuple;
		size_t matches;

		for (auto& compPair : entity.getComponents())
		{
			if (processEntityComponent<0, Components...>(compPair.first, compPair.second, compTuple))
			{
				matches++;
				if (matches == sizeof...(Components))
				{
					m_components.emplace_back(std::move(compTuple));
					m_idToIndexMap.emplace(entity.getID(), m_components.size() - 1);
					break;
				}
			}
		}
	}

	template<class... Components>
	inline void System<Components...>::onEntityDestroyed(EntityID entityID)
	{
		const auto fintIt = m_idToIndexMap.find(entityID);
		if (findIt != m_idToIndexMap.end())
		{
			// move the back element to overwrite this one
			m_components[findIt->second] = std::move(m_components.back());
			m_components.pop_back();

			// we need to figure out which entity the moved component belongs to so we can update the id to index 
			// map. Since all components in a single tuple are owned by the same entity, we can just grab the first
			// one and query its id.
			const auto* movedComp = std::get<0>(m_components[findIt->second]);

			// find the entry for the moved id and update it with the new index
			auto movedTupleIt = m_idToIndexMap.find(movedComp->getEntityID());
			DEBUG_ASSERT(movedTupleIt != m_idToIndexMap.end()); // should be impossible
			movedTupleIt->second = findIt->second;
		}
	}

	template<class... Components>
	template<size_t i, class CompType, class ...CompArgs>
	inline bool System<Components...>::processEntityComponent(ComponentID id, Component* component, ComponentTuple& tupleToFill)
	{
		if (CompType::ID == id)
		{
			std::get<i>(tupleToFill) = static_cast<CompType*>(component);
			return true;
		}

		return processEntityComponent<i + 1, CompArgs...>(id, component, tupleToFill);
	}

	template<class... Components>
	template<size_t i>
	inline bool System<Components...>::processEntityComponent(ComponentID id, Component* component, ComponentTuple& tupleToFill)
	{
		return false;
	}
}
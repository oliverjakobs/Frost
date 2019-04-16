#pragma once

#include "ecsEntity.h"
#include "ecsComponent.h"

#include <vector>

namespace ecs
{
	class BaseSystem;

	class Registry
	{
	private:
		using Entities = std::unordered_map<EntityID, Entity>;
		using Components = std::vector<std::vector<Component*>>;
		using Systems = std::vector<BaseSystem*>;

		Entities m_entities;
		Components m_components;
		Systems m_systems;

	public:
		// ecsRegistry is not copyable or movable
		Registry(const Registry&) = delete;
		Registry& operator=(const Registry&) = delete;
		Registry(Registry&&) = delete;
		Registry& operator=(Registry&&) = delete;

		Registry();
		~Registry();

		bool init();
		void update();

		EntityID createEntity();
		void destroyEntity(EntityID id);
	};
}
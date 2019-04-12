#pragma once

#include "ecsEntity.h"
#include "ecsComponent.h"

#include <vector>

class ecsBaseSystem;

class ECS
{
private:
	using Entities = std::unordered_map<ecsEntityID, ecsEntity>;
	using Components = std::vector<std::vector<ecsComponent*>>;
	using Systems = std::vector<ecsBaseSystem*>;

	Entities m_entities;
	Components m_components;
	Systems m_systems;

public:
	// ECS is not copyable or movable
	ECS(const ECS&) = delete;
	ECS& operator=(const ECS&) = delete;
	ECS(ECS&&) = delete;
	ECS& operator=(ECS&&) = delete;

	ECS();
	~ECS();

	bool init();
	void update();

	ecsEntityID createEntity();
	void destroyEntity(ecsEntityID id);
};
#pragma once

#include "ecsTypes.h"
#include "ecsEntity.h"

#include <vector>
#include <unordered_map>

class ecsSystem;

class ECS
{
private:
	using Entities = std::unordered_map<ecsEntityID, ecsEntity>;
	using Components = std::vector<std::vector<ecsComponent*>>;
	using Systems = std::vector<ecsSystem>;


	Entities m_entities;
	Components m_components;
	Systems m_systems;

public:
	// ECS is not copyable or moveable
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
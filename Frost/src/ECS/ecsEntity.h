#pragma once

#include "ecsTypes.h"
#include "Log/Logger.h"

#include <unordered_map>

class ecsComponent;

class ecsEntity
{
private:
	using ComponentMap = std::unordered_map<ecsComponentID, ecsComponent*, std::hash<ecsComponentID>, std::equal_to<ecsComponentID>>;

	ecsEntityID m_id;
	ComponentMap m_components;

public:
	explicit ecsEntity(ecsEntityID id) : m_id(id) {}
	// ecsEntity is not copyable but movable
	ecsEntity(const ecsEntity&) = delete;
	ecsEntity& operator=(const ecsEntity&) = delete;
	ecsEntity(ecsEntity&&) = default;
	ecsEntity& operator=(ecsEntity&&) = default;
	~ecsEntity() = default;

	void addComponent(ecsComponentID compID, ecsComponent* comp)
	{
		DEBUG_ASSERT(m_components.find(compID) == m_components.end(), "Component type already used ({0})", compID);
		m_components.emplace(compID, comp);
	}


	ecsEntityID getID() const { return m_id; }
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
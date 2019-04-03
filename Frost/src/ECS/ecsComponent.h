#pragma once

#include "ecsTypes.h"

#include <limits>

class ecsComponent
{
private:
	static constexpr size_t INVALID_POOL_INDEX = std::numeric_limits<size_t>::max();

	ecsEntityID m_entityID;
	size_t m_poolIndex;
public:
	ecsComponent() : m_entityID(INVALID_ENTITY_ID), m_poolIndex(INVALID_POOL_INDEX) {}
	virtual ~ecsComponent() = default;

	virtual bool init(void* desc) = 0;

	void setEntityID(ecsEntityID id) { m_entityID = id; }
	ecsEntityID getEntityID() const { return m_entityID; }

	void setPoolIndex(size_t index) { m_poolIndex = index; }
	size_t getPoolIndex() const { return m_poolIndex; }
};
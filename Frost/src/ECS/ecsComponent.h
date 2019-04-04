#pragma once

#include "ecsTypes.h"

#include "Utility/Memory/MemoryMacros.h"
#include "Utility/String/StringHash.h"

#define GENERATE_COMPONENT_TYPE_ID(_class_) static constexpr ecsComponentID ID = GenerateHash(#_class_);

class ecsComponent
{
private:
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
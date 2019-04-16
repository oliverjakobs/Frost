#pragma once

#include "ecsTypes.h"

#include "Utility/Utils.h"
#include "Utility/Memory/MemoryMacros.h"
#include "Utility/String/StringHash.h"

#define GENERATE_COMPONENT_TYPE_ID(_class_) static constexpr ecs::ComponentID ID = GenerateHash(#_class_);

namespace ecs
{

	class Component
	{
	private:
		EntityID m_entityID;
		size_t m_poolIndex;

	public:
		Component() : m_entityID(INVALID_ENTITY_ID), m_poolIndex(INVALID_POOL_INDEX) {}
		virtual ~Component() = default;

		virtual bool init(void* desc) = 0;

		void setEntityID(EntityID id) { m_entityID = id; }
		EntityID getEntityID() const { return m_entityID; }

		void setPoolIndex(size_t index) { m_poolIndex = index; }
		size_t getPoolIndex() const { return m_poolIndex; }
	};
}
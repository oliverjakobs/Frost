#pragma once

#include <limits>

namespace ecs
{
	typedef unsigned int EntityID;
	constexpr EntityID INVALID_ENTITY_ID = 0;

	typedef unsigned int ComponentID;
	constexpr size_t INVALID_POOL_INDEX = std::numeric_limits<size_t>::max();
}
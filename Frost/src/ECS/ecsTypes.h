#pragma once

#include <limits>

typedef unsigned int ecsEntityID;
constexpr ecsEntityID INVALID_ENTITY_ID = 0;

typedef unsigned int ecsComponentID;
constexpr size_t INVALID_POOL_INDEX = std::numeric_limits<size_t>::max();

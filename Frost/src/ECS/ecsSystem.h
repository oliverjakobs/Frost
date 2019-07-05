#pragma once

#include <entt/entt.hpp>
#include "Components.h"

struct ecsSystem
{
	virtual void Tick(entt::registry& registry) = 0;
};
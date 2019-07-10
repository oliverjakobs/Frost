#pragma once

#include "Components.h"

struct EntityManager
{
	static Entity* CreateEntity(Scene* scene, const std::string& path);
};
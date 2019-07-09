#pragma once

#include "Scene.h"

struct EntityManager
{
	static Entity* CreateEntity(Scene* scene, const std::string& path);
};
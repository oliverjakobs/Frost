#pragma once

#include "Components.h"

class Scene;

struct EntityManager
{
	static Entity* CreateEntity(Scene* scene, const std::string& path);
};
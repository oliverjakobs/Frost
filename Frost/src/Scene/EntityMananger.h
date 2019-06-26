#pragma once

#include "Scene.h"

struct EntityManager
{
	static unsigned int CreateEntity(Scene* scene, const std::string& path);
};
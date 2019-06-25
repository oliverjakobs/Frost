#pragma once

#include "Scene.h"

struct EntityManager
{
	static int CreateEntity(Scene* scene, const char* xmlPath);
};
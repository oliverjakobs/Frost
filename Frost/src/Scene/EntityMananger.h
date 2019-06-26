#pragma once

#include "Scene.h"

struct EntityManager
{
	static uint32_t CreateEntityXML(Scene* scene, const std::string& path);
	static uint32_t CreateEntityJSON(Scene* scene, const std::string& path);
};
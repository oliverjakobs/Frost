#pragma once

#include "Entity/Scene.h"

struct TemplateLoader
{
	static std::shared_ptr<Entity> LoadEntity(const std::string& path);
};
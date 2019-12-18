#pragma once

#include "Entity/Components.h"

struct EntityLoader
{
	static std::shared_ptr<Entity> LoadTemplate(const std::string& path);
};
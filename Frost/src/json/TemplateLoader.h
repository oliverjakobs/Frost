#pragma once

#include "Entity/Scene.h"

struct TemplateLoader
{
	static std::shared_ptr<Entity> LoadEntity(const std::string& path);
	static std::shared_ptr<Scene> LoadScene(const std::string& path, std::shared_ptr<ignis::Camera> camera);
};
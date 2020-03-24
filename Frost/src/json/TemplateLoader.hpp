#pragma once

#include "SceneManager.hpp"
#include "ResourceManager.hpp"

struct TemplateLoader
{
	static std::shared_ptr<Entity> LoadEntity(const std::string& path, ResourceManager* res);
	static std::shared_ptr<Scene> LoadScene(const std::string& path, std::shared_ptr<Camera> camera, ResourceManager* res);

	static void LoadSceneRegister(SceneManager* manager, const std::string& path);
	static void LoadResourceManager(ResourceManager* manager, const std::string& path);
};
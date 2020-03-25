#pragma once

#include "SceneManager.hpp"
#include "ResourceManager.hpp"

struct TemplateLoader
{
	static std::shared_ptr<Entity> LoadEntity(const char* path, ResourceManager* res);
	static std::shared_ptr<Scene> LoadScene(const char* path, std::shared_ptr<Camera> camera, ResourceManager* res);

	static void LoadSceneRegister(SceneManager* manager, const char* path);
	static void LoadResourceManager(ResourceManager* manager, const char* path);
};
#pragma once

#include "SceneManager.hpp"

struct TemplateLoader
{
	static std::shared_ptr<Entity> LoadEntity(const std::string& path);
	static std::shared_ptr<Scene> LoadScene(const std::string& path, std::shared_ptr<Camera> camera);

	static void LoadSceneRegister(SceneManager* manager, const std::string& path);
};
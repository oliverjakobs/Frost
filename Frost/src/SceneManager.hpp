#pragma once

#include "Entity/Scene.hpp"

#include "ResourceManager.h"

#include <map>

struct SceneManager
{
	Camera* camera;
	ResourceManager* resources;

	std::map<std::string, std::string> scenes;

	std::shared_ptr<Scene> scene;
	std::string sceneName;

	// editor
	bool editmode;
	bool showgrid;
	int layer;

	float gridsize;
	float padding;

	Entity* hover;
};

int SceneManagerInit(SceneManager* manager, ResourceManager* resources, Camera* camera, float gridsize, uint16_t padding);
void SceneManagerDelete(SceneManager* manager);

void SceneManagerRegisterScenes(SceneManager* manager, const std::string& path);

void SceneManagerRegisterScene(SceneManager* manager, const std::string& name, const std::string& path);
void SceneManagerChangeScene(SceneManager* manager, const std::string& name);

std::shared_ptr<Scene> SceneManagerLoadScene(SceneManager* manager, const std::string& name);

void SceneManagerOnEvent(SceneManager* manager, const Event& e);
void SceneManagerOnUpdate(SceneManager* manager, float deltaTime);
void SceneManagerOnRender(SceneManager* manager);
void SceneManagerOnRenderDebug(SceneManager* manager);
void SceneManagerOnImGui(SceneManager* manager);
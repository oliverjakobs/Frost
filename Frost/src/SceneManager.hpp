#pragma once

#include "Scene.hpp"

#include "ResourceManager.h"

#define SCENE_MANAGER_NAMELEN	32
#define SCENE_MANAGER_PATHLEN	64

#define SCENE_MANAGER_LAYER_COUNT	4

struct SceneManager
{
	Camera* camera;
	ResourceManager* resources;

	clib_hashmap scenes; /* <str,str> */

	Scene* scene;
	char scene_name[SCENE_MANAGER_NAMELEN];

	// editor
	bool editmode;
	bool showgrid;
	int layer;

	float gridsize;
	float padding;

	EcsEntity* hover;
};

int SceneManagerInit(SceneManager* manager, ResourceManager* resources, Camera* camera, float gridsize, uint16_t padding);
void SceneManagerDelete(SceneManager* manager);

void SceneManagerRegisterScenes(SceneManager* manager, const char* path);

void SceneManagerRegisterScene(SceneManager* manager, const char* name, const char* path);
void SceneManagerChangeScene(SceneManager* manager, const char* name);

int SceneManagerLoadScene(SceneManager* manager, Scene* scene, const char* name);

void SceneManagerOnEvent(SceneManager* manager, const Event& e);
void SceneManagerOnUpdate(SceneManager* manager, float deltaTime);
void SceneManagerOnRender(SceneManager* manager);
void SceneManagerOnRenderDebug(SceneManager* manager);
void SceneManagerOnRenderGui(SceneManager* manager);
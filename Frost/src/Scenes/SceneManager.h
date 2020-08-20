#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "ECS/Systems.h"
#include "Application/Application.h"

#include "Graphics/Background.h"
#include "clib/strmap.h"

typedef struct 
{
	Camera* camera;
	ResourceManager* resources;

	clib_hashmap scenes;
	clib_hashmap templates;

	char scene_name[APPLICATION_STR_LEN];
	float width;
	float height;

	Background background;

	Ecs ecs;
} SceneManager;

int SceneManagerInit(SceneManager* manager, const char* reg, ResourceManager* resources, Camera* camera, float gridsize, uint16_t padding);
void SceneManagerDestroy(SceneManager* manager);

void SceneManagerChangeScene(SceneManager* manager, const char* name);

int SceneManagerLoadScene(SceneManager* manager, float w, float h);
void SceneManagerQuitScene(SceneManager* manager);

void SceneManagerOnEvent(SceneManager* manager, Event e);
void SceneManagerOnUpdate(SceneManager* manager, float deltatime);
void SceneManagerOnRender(SceneManager* manager);
void SceneManagerOnRenderDebug(SceneManager* manager);

#endif /* !SCENE_MANAGER_H */
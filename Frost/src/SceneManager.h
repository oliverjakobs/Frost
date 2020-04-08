#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Scene.h"
#include "ECS/TemplateLoader.h"

#include "Application/defines.h"

#define SCENE_MANAGER_LAYER_COUNT	4

typedef struct 
{
	Camera* camera;
	ResourceManager* resources;

	clib_hashmap scenes; /* <str,str> */

	Scene* scene;
	char scene_name[APPLICATION_STR_LEN];

	// editor
	bool editmode;
	bool showgrid;
	int layer;

	float gridsize;
	float padding;

	EcsEntity* hover;
} SceneManager;

int SceneManagerInit(SceneManager* manager, ResourceManager* resources, Camera* camera, float gridsize, uint16_t padding);
void SceneManagerDelete(SceneManager* manager);

void SceneManagerRegisterScenes(SceneManager* manager, const char* path);

void SceneManagerRegisterScene(SceneManager* manager, const char* name, const char* path);
void SceneManagerChangeScene(SceneManager* manager, const char* name);

int SceneManagerLoadScene(SceneManager* manager, Scene* scene, const char* name);

void SceneManagerOnEvent(SceneManager* manager, Event e);
void SceneManagerOnUpdate(SceneManager* manager, float deltaTime);
void SceneManagerOnRender(SceneManager* manager);
void SceneManagerOnRenderDebug(SceneManager* manager);
void SceneManagerOnRenderGui(SceneManager* manager);

#ifdef __cplusplus
}
#endif

#endif /* !SCENE_MANAGER_H */
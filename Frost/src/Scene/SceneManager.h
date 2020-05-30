#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Scene.h"
#include "SceneEditor.h"

#include "Console/Console.h"

#include "Graphics/ResourceManager.h"

#include "clib/strmap.h"

#define SCENE_MANAGER_LAYER_COUNT	4

typedef struct 
{
	Camera* camera;
	ResourceManager* resources;

	clib_strmap scenes; /* <str,str> */
	clib_strmap templates; /* <str,str> */

	Scene* scene;
	char scene_name[APPLICATION_STR_LEN];

	SceneEditor editor;
	int editmode;

	Console console;
} SceneManager;

int SceneManagerInit(SceneManager* manager, const char* reg, ResourceManager* resources, Camera* camera, float gridsize, uint16_t padding);
void SceneManagerDestroy(SceneManager* manager);

void SceneManagerChangeScene(SceneManager* manager, const char* name);

void SceneManagerOnEvent(SceneManager* manager, Event e);
void SceneManagerOnUpdate(SceneManager* manager, float deltatime);
void SceneManagerOnRender(SceneManager* manager);
void SceneManagerOnRenderDebug(SceneManager* manager);
void SceneManagerOnRenderGui(SceneManager* manager);

#ifdef __cplusplus
}
#endif

#endif /* !SCENE_MANAGER_H */
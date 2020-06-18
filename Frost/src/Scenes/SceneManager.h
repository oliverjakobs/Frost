#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

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

	clib_hashmap scenes;
	clib_hashmap templates;

	Scene* scene;
	char scene_name[APPLICATION_STR_LEN];

	SceneEditor editor;
	int editmode;

	Console console;
	int console_focus;
} SceneManager;

int SceneManagerInit(SceneManager* manager, const char* reg, ResourceManager* resources, Camera* camera, float gridsize, uint16_t padding);
void SceneManagerDestroy(SceneManager* manager);

void SceneManagerFocusConsole(SceneManager* manager);
void SceneManagerToggleEditmode(SceneManager* manager);

void SceneManagerChangeScene(SceneManager* manager, const char* name);

void SceneManagerExecuteCommand(SceneManager* manager, char* cmd_buffer);

void SceneManagerOnEvent(SceneManager* manager, Event e);
void SceneManagerOnUpdate(SceneManager* manager, float deltatime);
void SceneManagerOnRender(SceneManager* manager);
void SceneManagerOnRenderDebug(SceneManager* manager);
void SceneManagerOnRenderGui(SceneManager* manager);

#endif /* !SCENE_MANAGER_H */
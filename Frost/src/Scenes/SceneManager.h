#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Frost/FrostEcs.h"
#include "Application/Application.h"

#include "Graphics/Background.h"
#include "clib/strmap.h"

typedef struct 
{
	Camera* camera;
	Resources* resources;

	clib_hashmap scenes;
	clib_hashmap templates;

	Ecs ecs;

	/* active scene specific data */
	char scene_name[APPLICATION_STR_LEN];
	float width;
	float height;

	Background background;

} SceneManager;

int SceneManagerInit(SceneManager* manager, const char* reg, Resources* resources, Camera* camera, float gridsize, uint16_t padding);
void SceneManagerDestroy(SceneManager* manager);

void SceneManagerChangeActive(SceneManager* manager, const char* name);

void SceneManagerClearActive(SceneManager* manager);

void SceneManagerOnEvent(SceneManager* manager, Event e);
void SceneManagerOnUpdate(SceneManager* manager, float deltatime);
void SceneManagerOnRender(SceneManager* manager);
void SceneManagerOnRenderDebug(SceneManager* manager);

#endif /* !SCENE_MANAGER_H */
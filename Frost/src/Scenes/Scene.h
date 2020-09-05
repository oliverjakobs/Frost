#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Application/Application.h"
#include "Graphics/Background.h"

#include "ECS/Ecs.h"

typedef struct 
{
	/* register */
	clib_hashmap scene_register;
	clib_hashmap templates;

	Camera* camera;
	Resources* resources;

	Ecs ecs;
	Background background;

	/* active scene specific data */
	char name[APPLICATION_STR_LEN];
	float width;
	float height;
} Scene;

int SceneInit(Scene* scene, Camera* camera, const char* path, Resources* resources);
void SceneDestroy(Scene* scene);

void SceneChangeActive(Scene* scene, const char* name);
void SceneClearActive(Scene* scene);

void SceneOnEvent(Scene* scene, Event e);
void SceneOnUpdate(Scene* scene, float deltatime);
void SceneOnRender(Scene* scene);
void SceneOnRenderDebug(Scene* scene);

int SceneLoad(Scene* scene, const char* path);
int SceneSave(Scene* scene, const char* path);

int SceneLoadTemplate(Scene* scene, const char* templ, EntityID entity, vec2 pos, int z_index);

#endif /* !SCENE_MANAGER_H */
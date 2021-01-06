#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Application/Application.h"
#include "Graphics/Background.h"

#include "Camera/Camera.h"

#include "ECS/Ecs.h"

#include "tile/tilemap.h"

typedef struct 
{
	/* register */
	tb_hashmap scene_register;
	tb_hashmap templates;

	Ecs ecs;
	Camera* camera;
	Resources* resources;
	TileMap map;

	/* active scene specific data */
	char name[APPLICATION_STR_LEN];
	float width;
	float height;

	Background background;
} Scene;

int SceneInit(Scene* scene, Camera* camera, Resources* resources, int (*load)(Ecs* ecs));
int SceneLoadScenes(Scene* scene, const char* reg, const char* start);
void SceneDestroy(Scene* scene);

void SceneChangeActive(Scene* scene, const char* name);
void SceneClearActive(Scene* scene);

void SceneOnEvent(Scene* scene, Event e);
void SceneOnUpdate(Scene* scene, float deltatime);
void SceneOnRender(Scene* scene);
void SceneOnRenderUI(Scene* scene);
void SceneOnRenderDebug(Scene* scene);

int SceneLoad(Scene* scene, const char* path);
int SceneSave(Scene* scene, const char* path);

int SceneLoadTemplate(Scene* scene, const char* templ, EcsEntityID entity, vec2 pos, int z_index);

const char* SceneGetTemplatePath(const Scene* scene, const char* templ);

#endif /* !SCENE_MANAGER_H */
#ifndef SCENE_H
#define SCENE_H

#include "Application/Application.h"
#include "Graphics/Background.h"

#include "Camera/Camera.h"

#include "ECS/Ecs.h"
#include "tile/tile_map.h"
#include "tile/tile_renderer.h"

typedef struct 
{
	/* register */
	tb_hashmap scene_register;
	tb_hashmap templates;

	Ecs ecs;
	TileMap map;
	vec2 gravity;
	TileRenderer renderer;

	Camera camera;
	Resources* resources;

	/* active scene specific data */
	char name[APPLICATION_STR_LEN];
	IgnisTexture2D* tile_set;

	Background background;
} Scene;

int SceneInit(Scene* scene, float camera_w, float camera_h, Resources* resources, int (*load)(Ecs* ecs));
int SceneLoadScenes(Scene* scene, const char* reg, const char* start);
void SceneDestroy(Scene* scene);

void SceneChangeActive(Scene* scene, const char* name);
void SceneClearActive(Scene* scene);

void SceneOnEvent(Scene* scene, Event e);
void SceneOnUpdate(Scene* scene, float deltatime);
void SceneOnRender(Scene* scene);
void SceneOnRenderUI(Scene* scene);
void SceneOnRenderDebug(Scene* scene);

const char* SceneGetTemplatePath(const Scene* scene, const char* templ);

float SceneGetWidth(const Scene* scene);
float SceneGetHeight(const Scene* scene);

#endif /* !SCENE_H */
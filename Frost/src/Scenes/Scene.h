#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Resources.h"

#include "Graphics/Background.h"
#include "Minimal/Minimal.h"

#include "ECS/Ecs.h"
#include "tile/tile_map.h"
#include "tile/tile_renderer.h"

#include "toolbox/tb_mem.h"

typedef struct Scene Scene;

typedef (*SceneLoadFn)(Scene*, const char*);
typedef (*SceneSaveFn)(Scene*, const char*);

struct Scene
{
	Camera camera;

	Ecs ecs;
	TileMap map;
	TileRenderer renderer;

	SceneLoadFn load;
	SceneSaveFn save;
	tb_allocator* allocator;
	
	/* active scene specific data */
	char path[APPLICATION_PATH_LEN];

	vec2 gravity;

	Resources res;
	IgnisTexture2D item_atlas;
	IgnisTexture2D tile_set;

	Background background;
};

int SceneInit(Scene* scene, vec2 screen_size, SceneLoadFn load, SceneSaveFn save, tb_allocator* allocator);
void SceneDestroy(Scene* scene);

void SceneChangeActive(Scene* scene, const char* path, int reload);
void SceneClearActive(Scene* scene);

void SceneOnEvent(Scene* scene, Event e);
void SceneOnUpdate(Scene* scene, float deltatime);
void SceneOnRender(Scene* scene);
void SceneOnRenderUI(Scene* scene);
void SceneOnRenderDebug(Scene* scene);

float SceneGetWidth(const Scene* scene);
float SceneGetHeight(const Scene* scene);

#endif /* !SCENE_H */
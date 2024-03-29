#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Resources.h"
#include "Background.h"

#include "Minimal/Event.h"

#define APPLICATION_STR_LEN     32
#define APPLICATION_PATH_LEN    64
#define APPLICATION_VER_STR_LEN 8

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
    IgnisTexture2D* item_atlas;
    vec2i item_atlas_size;
    IgnisTexture2D* tile_set;
    vec2i tile_set_size;

    BackgroundLayer* background;
};

int SceneInit(Scene* scene, float x, float y, SceneLoadFn load, SceneSaveFn save, tb_allocator* allocator);
void SceneDestroy(Scene* scene);

void SceneChangeActive(Scene* scene, const char* path, int reload);
void SceneClearActive(Scene* scene);

void SceneOnEvent(Scene* scene, const MinimalEvent* e);
void SceneOnUpdate(Scene* scene, float deltatime);
void SceneOnRender(Scene* scene);
void SceneOnRenderUI(Scene* scene);
void SceneOnRenderDebug(Scene* scene);

float SceneGetWidth(const Scene* scene);
float SceneGetHeight(const Scene* scene);

#endif /* !SCENE_H */
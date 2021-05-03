#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include "Scenes/Scene.h"

#define FROST_TEMPLATE_SIZE 1024

typedef enum
{
    SCENE_LOAD_OK,
    SCENE_LOAD_READ_ERROR,
    SCENE_LOAD_MAP_ERROR,
    SCENE_LOAD_MAP_SIZE_ERROR,
    SCENE_LOAD_STREAM_MAP_ERROR,
    SCENE_LOAD_STREAM_TYPES_ERROR,
} SceneLoadError;

int SceneLoad(Scene* scene, const char* path);
int SceneLoadTemplate(Scene* scene, const char* templ, vec2 pos, int z_index, int variant);

SceneLoadError SceneLoadTextures(Scene* scene, char* json);
SceneLoadError SceneLoadMap(Scene* scene, char* json);
SceneLoadError SceneLoadSaveState(Scene* scene, char* json);

const char* SceneLoadErrorDesc(SceneLoadError error);

#endif // !SCENE_LOADER_H

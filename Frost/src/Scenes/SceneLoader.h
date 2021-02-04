#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include "Scene.h"

int SceneLoad(Scene* scene, const char* path);
int SceneSave(Scene* scene, const char* path);

int SceneLoadTemplate(Scene* scene, const char* templ, vec2 pos, int z_index, int variant);

#endif // !SCENE_LOADER_H

#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include "Scenes/Scene.h"

#define FROST_TEMPLATE_SIZE 1024

int SceneLoad(Scene* scene, const char* path);
int SceneLoadTemplate(Scene* scene, EcsEntityID entity, const char* templ, vec2 pos, int z_index, int variant);

int SceneLoadBackground(Scene* scene, char* ini);
int SceneLoadMap(Scene* scene, char* ini);
int SceneLoadState(Scene* scene, char* ini);

#endif // !SCENE_LOADER_H

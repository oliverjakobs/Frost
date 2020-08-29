#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include "SceneManager.h"

int ScenesLoadRegister(SceneManager* manager, const char* path);

int ScenesLoadScene(SceneManager* manager, const char* path);
int ScenesSaveScene(SceneManager* manager, const char* path);

int ScenesLoadTemplate(SceneManager* manager, const char* templ, EntityID entity, vec2 pos, int z_index);

#endif /* !SCENE_LOADER_H */


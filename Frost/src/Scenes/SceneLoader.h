#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include "SceneManager.h"

int SceneLoaderLoadRegister(SceneManager* manager, const char* path);

int SceneLoaderLoadScene(SceneManager* manager, const char* path);
int SceneLoaderSaveScene(SceneManager* manager, const char* path);

int SceneLoaderLoadTemplate(SceneManager* manager, const char* templ, EntityID entity, vec2 pos, int z_index);

#endif /* !SCENE_LOADER_H */


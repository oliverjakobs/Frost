#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include "SceneManager.h"

int SceneLoaderLoadRegister(SceneManager* manager, const char* path);

int SceneLoaderLoadScene(Scene* scene, const char* path, Camera* camera, ResourceManager* resources, clib_strmap* templates);
int SceneLoaderSaveScene(Scene* scene, const char* path, ResourceManager* resources);

int SceneLoaderLoadTemplate(const char* entity, const char* path, ComponentTable* components, ResourceManager* resources);

#endif /* !SCENE_LOADER_H */


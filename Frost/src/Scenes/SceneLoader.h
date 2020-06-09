#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include "SceneManager.h"

int SceneLoaderLoadRegister(SceneManager* manager, const char* path);

int SceneLoaderLoadScene(Scene* scene, const char* path, Camera* camera, ResourceManager* resources, clib_strmap* templates);
int SceneLoaderSaveScene(Scene* scene, const char* path, ResourceManager* resources);

EcsEntity* SceneLoaderLoadTemplate(const char* name, const char* templ, clib_strmap* templates, ComponentTable* components, ResourceManager* resources);

#endif /* !SCENE_LOADER_H */


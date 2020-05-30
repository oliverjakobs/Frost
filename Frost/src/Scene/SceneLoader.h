#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include "SceneManager.h"

void SceneLoaderLoadRegister(SceneManager* manager, const char* path);

int SceneLoaderLoadScene(SceneManager* manager, Scene* scene, const char* json);
int SceneLoaderSaveScene(SceneManager* manager, Scene* scene, const char* path);

char* SceneLoaderGetTemplate(SceneManager* manager, const char* name);

#endif /* !SCENE_LOADER_H */


#ifndef SCENE_SAVER_H
#define SCENE_SAVER_H

#include "Scenes/Scene.h"

int SceneSave(Scene* scene, const char* path);

int SceneSaveMap(Scene* scene, FILE* const stream);
int SceneSaveState(Scene* scene, FILE* const stream);

#endif /* !SCENE_SAVER_H */
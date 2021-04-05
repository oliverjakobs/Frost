#ifndef SCENE_SAVER_H
#define SCENE_SAVER_H

#include "Scenes/Scene.h"
#include "toolbox/tb_jwrite.h"

typedef enum
{
    SCENE_SAVE_OK,
    SCENE_SAVE_MAP_ERROR
} SceneSaveError;

int SceneSave(Scene* scene, const char* path);

SceneSaveError SceneSaveMap(Scene* scene, tb_jwrite_control* jwc);

#endif /* !SCENE_SAVER_H */
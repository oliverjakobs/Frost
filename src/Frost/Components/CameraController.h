#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "Scenes/Scene.h"

/*
TEMPLATE:
[camera]
smooth = <float : 0.0>
*/

typedef struct
{
    Camera* camera;
    vec2 scene_size;
    float smooth;
} CameraController;

void CameraControllerLoad(char* ini, Ecs* ecs, EcsEntityID entity, Scene* scene);

void CameraControllerMoveConstrained(CameraController* controller, vec2 pos, float constraint);

#endif /* !CAMERA_CONTROLLER_H */

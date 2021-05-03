#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "Scenes/Scene.h"

typedef struct
{
	Camera* camera;
	vec2 scene_size;
	float smooth;
} CameraController;

void CameraControllerLoad(char* json, Ecs* ecs, EcsEntityID entity, Scene* scene);

void CameraControllerMoveConstrained(CameraController* controller, vec2 pos, float constraint);

#endif /* !CAMERA_CONTROLLER_H */

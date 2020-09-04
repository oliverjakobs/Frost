#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "Scenes/Scene.h"

typedef struct
{
	Camera* camera;
	float smooth;
	float scene_w;
	float scene_h;
} CameraController;

void CameraControllerLoad(Scene* scene, EntityID entity, char* json);

#endif /* !CAMERA_CONTROLLER_H */

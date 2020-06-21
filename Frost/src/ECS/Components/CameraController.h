#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "Camera/Camera.h"

typedef struct
{
	Camera* camera;
	float smooth;
	float scene_w;
	float scene_h;
} CameraController;

#endif /* !CAMERA_CONTROLLER_H */

#ifndef CAMERA_H
#define CAMERA_H

#include "math/math.h"

typedef struct
{
	mat4 view;
	mat4 projection;
	mat4 viewProjection;

	vec3 position;
	vec2 size;
} Camera;

void CameraCreate(Camera* camera, float x, float y, float z, float w, float h);
void CameraCreateOrtho(Camera* camera, float x, float y, float z, float w, float h);
void CameraCreateVec(Camera* camera, vec3 pos, vec2 size);
void CameraCreateOrthoVec(Camera* camera, vec3 center, vec2 size);

/*
 * Call after changing size or position
 */
void CameraUpdateViewOrtho(Camera* camera);

void CameraSetProjectionOrtho(Camera* camera, float w, float h);
void CameraSetProjectionOrthoVec2(Camera* camera, vec2 size);

const vec2 CameraGetMousePos(Camera* camera, vec2 mouse);

const float* CameraGetViewPtr(Camera* camera);
const float* CameraGetProjectionPtr(Camera* camera);
const float* CameraGetViewProjectionPtr(Camera* camera);

#endif /* !CAMERA_H */
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

vec2 CameraGetMousePos(const Camera* camera, vec2 mouse);
vec2 CameraGetMousePosView(const Camera* camera, vec2 mouse);

const float* CameraGetViewPtr(const Camera* camera);
const float* CameraGetProjectionPtr(const Camera* camera);
const float* CameraGetViewProjectionPtr(const Camera* camera);

#endif /* !CAMERA_H */
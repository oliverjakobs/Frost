#include "Camera.h"

void CameraCreate(Camera* camera, float x, float y, float z, float w, float h)
{
	camera->view = mat4_indentity();
	camera->projection = mat4_indentity();
	camera->viewProjection = mat4_indentity();

	camera->position.x = x;
	camera->position.y = y;
	camera->position.z = z;
	camera->size.x = w;
	camera->size.y = h;
}

void CameraCreateOrtho(Camera* camera, float x, float y, float z, float w, float h)
{
	CameraCreate(camera, x, y, z, w, h);
	CameraSetProjectionOrtho(camera, w, h);
}

void CameraCreateVec(Camera* camera, vec3 pos, vec2 size)
{
	camera->view = mat4_indentity();
	camera->projection = mat4_indentity();
	camera->viewProjection = mat4_indentity();

	camera->position = pos;
	camera->size = size;
}

void CameraCreateOrthoVec(Camera* camera, vec3 center, vec2 size)
{
	CameraCreateVec(camera, center, size);
	CameraSetProjectionOrtho(camera, size.x, size.y);
}

void CameraUpdateViewOrtho(Camera* camera)
{
	mat4 transform = mat4_indentity();
	transform = mat4_translate(transform, camera->position);

	camera->view = mat4_inverse(transform);
	camera->viewProjection = mat4_multiply(camera->projection, camera->view);
}

void CameraSetProjectionOrtho(Camera* camera, float w, float h)
{
	camera->projection = mat4_ortho(0.0f, w, 0.0f, h, -1.0f, 1.0f);

	camera->size.x = w;
	camera->size.y = h;

	CameraUpdateViewOrtho(camera);
}

void CameraSetProjectionOrthoVec2(Camera* camera, vec2 size)
{
	CameraSetProjectionOrtho(camera, size.x, size.y);
}

void CameraSetCenter(Camera* camera, vec2 center)
{
	camera->position.x = center.x - camera->size.x * 0.5f;
	camera->position.y = center.y - camera->size.y * 0.5f;

	CameraUpdateViewOrtho(camera);
}

vec2 CameraGetCenter(const Camera* camera)
{
	return vec2_add((vec2) { camera->position.x, camera->position.y }, vec2_mult(camera->size, 0.5f));
}

vec2 CameraGetMousePos(const Camera* camera, vec2 mouse)
{
	vec2 pos;
	pos.x = mouse.x;
	pos.y = (camera->size.y - mouse.y);

	return pos;
}

vec2 CameraGetMousePosView(const Camera* camera, vec2 mouse)
{
	vec2 pos;
	pos.x = mouse.x + camera->position.x;
	pos.y = (camera->size.y - mouse.y) + camera->position.y;

	return pos;
}

const float* CameraGetViewPtr(const Camera* camera)
{
	return camera->view.v;
}

const float* CameraGetProjectionPtr(const Camera* camera)
{
	return camera->projection.v;
}

const float* CameraGetViewProjectionPtr(const Camera* camera)
{
	return camera->viewProjection.v;
}

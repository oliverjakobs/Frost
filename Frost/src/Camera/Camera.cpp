#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

int CameraCreate(Camera* camera, const glm::vec3& pos, const glm::vec2& size)
{
	camera->view = glm::mat4(1.0f);
	camera->projection = glm::mat4(1.0f);
	camera->viewProjection = glm::mat4(1.0f);

	camera->position = pos;
	camera->size = size;

	return 1;
}

int CameraCreateOrtho(Camera* camera, const glm::vec3& center, const glm::vec2& size)
{
	int result = CameraCreate(camera, center, size);
	CameraSetProjectionOrtho(camera, size);

	return result;
}

void CameraUpdateViewOrtho(Camera* camera)
{
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, camera->position);

	camera->view = glm::inverse(transform);
	camera->viewProjection = camera->projection * camera->view;
}

void CameraSetProjectionOrtho(Camera* camera, float w, float h)
{
	camera->projection = glm::ortho(-w / 2.0f, w / 2.0f, -h / 2.0f, h / 2.0f, -1.0f, 1.0f);

	CameraUpdateViewOrtho(camera);
}

void CameraSetProjectionOrtho(Camera* camera, const glm::vec2& size)
{
	CameraSetProjectionOrtho(camera, size.x, size.y);
}

const glm::vec2 CameraGetMousePos(Camera* camera, const glm::vec2& mouse)
{
	float x = mouse.x + (camera->position.x - (camera->size.x / 2.0f));
	float y = (camera->size.y - mouse.y) + (camera->position.y - (camera->size.y / 2.0f));

	return glm::vec2(x, y);
}

const glm::vec2 CameraGetMousePos(Camera* camera, const vec2 mouse)
{
	float x = mouse.x + (camera->position.x - (camera->size.x / 2.0f));
	float y = (camera->size.y - mouse.y) + (camera->position.y - (camera->size.y / 2.0f));

	return glm::vec2(x, y);
}

const float* CameraGetViewPtr(Camera* camera)
{
	return &camera->view[0][0];
}

const float* CameraGetProjectionPtr(Camera* camera)
{
	return &camera->projection[0][0];
}

const float* CameraGetViewProjectionPtr(Camera* camera)
{
	return &camera->viewProjection[0][0];
}

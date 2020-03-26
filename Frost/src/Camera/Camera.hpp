#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

struct Camera
{
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 viewProjection;

	glm::vec3 position;
	glm::vec2 size;
};

int CameraCreate(Camera* camera, const glm::vec3& pos, const glm::vec2& size);
int CameraCreateOrtho(Camera* camera, const glm::vec3& center, const glm::vec2& size);

/*
 * Call after changing size or position
 */
void CameraUpdateViewOrtho(Camera* camera);

void CameraSetProjectionOrtho(Camera* camera, float w, float h);
void CameraSetProjectionOrtho(Camera* camera, const glm::vec2& size);

const glm::vec2 CameraGetMousePos(Camera* camera, const glm::vec2& mouse);

const float* CameraGetViewPtr(Camera* camera);
const float* CameraGetProjectionPtr(Camera* camera);
const float* CameraGetViewProjectionPtr(Camera* camera);

#endif /* !CAMERA_H */
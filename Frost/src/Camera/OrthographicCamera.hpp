#pragma once

#include "Camera.hpp"

class OrthographicCamera : public Camera
{
public:
	OrthographicCamera();
	OrthographicCamera(const glm::vec3& center, const glm::vec2& size);

	void SetProjection(float w, float h);
	void SetProjection(const glm::vec2& size);

private:
	void UpdateView() override;
};

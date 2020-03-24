#pragma once

#include <glm/glm.hpp>

class Camera
{
protected:
	glm::mat4 m_view;
	glm::mat4 m_projection;
	glm::mat4 m_viewProjection;

	glm::vec3 m_position;
	glm::vec2 m_size;

public:
	Camera();
	Camera(const glm::vec3& pos, const glm::vec2& size);

	void SetPosition(const glm::vec3& position);
	void SetSize(const glm::vec2& size);

	const glm::vec2 GetMousePos(const glm::vec2& mouse) const;

	const glm::vec3& GetPosition() const { return m_position; }
	const glm::vec2& GetSize() const { return m_size; }

	const glm::mat4& GetView() const { return m_view; }
	const glm::mat4& GetProjection() const { return m_projection; }
	const glm::mat4& GetViewProjection() const { return m_viewProjection; }

	const float* GetViewPtr() const { return &m_view[0][0]; }
	const float* GetProjectionPtr() const { return &m_projection[0][0]; }
	const float* GetViewProjectionPtr() const { return &m_viewProjection[0][0]; }

protected:
	virtual void UpdateView() = 0;
};
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
	Camera() : m_view(1.0f), m_projection(1.0f), m_viewProjection(1.0f), m_position(), m_size() {}
	Camera(const glm::vec3& pos, const glm::vec2& size) : m_view(1.0f), m_projection(1.0f), m_viewProjection(1.0f), m_position(pos), m_size(size) {}

	void SetPosition(const glm::vec3& position) { m_position = position; UpdateView(); }
	const glm::vec3& GetPosition() const { return m_position; }

	void SetSize(const glm::vec2& size) { m_size = size; UpdateView(); }
	const glm::vec2& GetSize() const { return m_size; }

	const glm::mat4& GetViewx() const { return m_view; }
	const glm::mat4& GetProjection() const { return m_projection; }
	const glm::mat4& GetViewProjection() const { return m_viewProjection; }

protected:
	virtual void UpdateView() = 0;
};
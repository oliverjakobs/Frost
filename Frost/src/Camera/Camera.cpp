#include "Camera.hpp"

Camera::Camera() : m_view(1.0f), m_projection(1.0f), m_viewProjection(1.0f), m_position(), m_size() 
{

}

Camera::Camera(const glm::vec3& pos, const glm::vec2& size) 
	: m_view(1.0f), m_projection(1.0f), m_viewProjection(1.0f), m_position(pos), m_size(size)
{

}

void Camera::SetPosition(const glm::vec3& position)
{ 
	m_position = position;
	UpdateView(); 
}

void Camera::SetSize(const glm::vec2& size)
{ 
	m_size = size;
	UpdateView();
}

const glm::vec2 Camera::GetMousePos(const glm::vec2& mouse) const
{
	float x = mouse.x + (m_position.x - (m_size.x / 2.0f));
	float y = (m_size.y - mouse.y) + (m_position.y - (m_size.y / 2.0f));

	return glm::vec2(x, y);
}

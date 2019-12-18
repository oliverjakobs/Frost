#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace ignis
{
	OrthographicCamera::OrthographicCamera()
	{
	}

	OrthographicCamera::OrthographicCamera(const glm::vec3& center, const glm::vec2& size)
		: Camera(center, size)
	{
		SetProjection(size);
	}

	void OrthographicCamera::SetProjection(const glm::vec2& size)
	{
		m_projection = glm::ortho(-size.x / 2.0f, size.x / 2.0f, -size.y / 2.0f, size.y / 2.0f, -1.0f, 1.0f);

		UpdateView();
	}

	void OrthographicCamera::UpdateView()
	{
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, m_position);

		m_view = glm::inverse(transform);
		m_viewProjection = m_projection * m_view;
	}
}
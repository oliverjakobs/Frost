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

	void OrthographicCamera::SetProjection(float w, float h)
	{
		m_projection = glm::ortho(-w / 2.0f, w / 2.0f, -h / 2.0f, h / 2.0f, -1.0f, 1.0f);

		UpdateView();
	}

	void OrthographicCamera::SetProjection(const glm::vec2& size)
	{
		SetProjection(size.x, size.y);
	}

	void OrthographicCamera::UpdateView()
	{
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, m_position);

		m_view = glm::inverse(transform);
		m_viewProjection = m_projection * m_view;
	}
}
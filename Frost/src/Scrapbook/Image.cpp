#include "Image.h"

#include <glm\gtc\matrix_transform.hpp>

namespace sb
{
	Image::Image(const std::string& src, float width, float height)
	{
		float vertices[] =
		{
			0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
			width, 0.0f, 0.0f,  1.0f, 0.0f,
			width, height, 0.0f,  1.0f, 1.0f,

			width, height, 0.0f,  1.0f, 1.0f,
			0.0f, height, 0.0f,  0.0f, 1.0f,
			0.0f, 0.0f, 0.0f,  0.0f, 0.0f
		};
		
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		m_vbo.bind();
		m_vbo.setBufferData(sizeof(vertices), vertices, GL_STATIC_DRAW);
		m_vbo.setAttribPointer(0, 3, 5, 0);
		m_vbo.setAttribPointer(1, 2, 5, 3);

		m_texture = new Texture(src.c_str(), 0);
	}

	Image::~Image()
	{
		if (m_texture != nullptr)
			delete m_texture;
		
		glDeleteVertexArrays(1, &m_vao);
	}

	void Image::render(float x, float y, Shader* shader)
	{
		shader->setUniformMat4("model", glm::translate(glm::mat4(), glm::vec3(x, y, 0.0f)));

		m_texture->bind();

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void Image::render(const glm::vec2& pos, Shader* shader)
	{
		render(pos.x, pos.y, shader);
	}
}
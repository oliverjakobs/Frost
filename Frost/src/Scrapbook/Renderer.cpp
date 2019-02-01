#include "Renderer.h"

namespace sb
{
	void Renderer::Init(float x, float y, float w, float h)
	{

		Get()->m_primitives.create();

		Get()->m_view = glm::ortho(x, w, y, h);
	}
	
	void Renderer::Start()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::Flush()
	{
		Get()->m_primitives.flush(GetView());
	}

	void Renderer::EnableBlend(GLenum sfactor, GLenum dfactor)
	{
		glEnable(GL_BLEND);
		glBlendFunc(sfactor, dfactor);
	}

	void Renderer::RenderTexture(Texture* tex, const glm::vec2& position, Shader* shader)
	{
		if (shader != nullptr)
		{
			shader->use();

			shader->setUniformMat4("projection", glm::mat4(1.0f));
			shader->setUniformMat4("view", GetView());
			shader->setUniformMat4("model", glm::translate(glm::mat4(), glm::vec3(position, 0.0f)));
		}

		tex->bind();

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void Renderer::DrawLine(const Line& line, const glm::vec4& color)
	{
		Get()->m_primitives.drawLine(line.start, line.end, color, GetView());
	}

	void Renderer::DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color)
	{
		Get()->m_primitives.drawLine(start, end, color, GetView());
	}

	void Renderer::DrawRect(float x, float y, float w, float h, const glm::vec4& color)
	{
		std::vector<glm::vec2> vertices =
		{
			glm::vec2(x, y),
			glm::vec2(x + w, y),
			glm::vec2(x + w, y + h),
			glm::vec2(x, y + h)
		};

		Get()->m_primitives.drawPolygon(vertices, color, GetView());
	}

	void Renderer::DrawRect(const glm::vec2& pos, const glm::vec2& dim, const glm::vec4& color)
	{
		DrawRect(pos.x, pos.y, dim.x, dim.y, color);
	}

	void Renderer::DrawCircle(const glm::vec2& center, float radius, const glm::vec4 & color)
	{
		Get()->m_primitives.drawCircle(center, radius, color, GetView());
	}

	void Renderer::DrawPolygon(const std::vector<glm::vec2>& vertices, const glm::vec4& color)
	{
		Get()->m_primitives.drawPolygon(vertices, color, GetView());
	}

	void Renderer::FillRect(float x, float y, float w, float h, const glm::vec4& color)
	{
		std::vector<glm::vec2> vertices =
		{
			glm::vec2(x, y),
			glm::vec2(x + w, y),
			glm::vec2(x + w, y + h),
			glm::vec2(x, y + h)
		};

		Get()->m_primitives.fillPolygon(vertices, color, GetView());
	}
	
	void Renderer::FillCircle(const glm::vec2& center, float radius, const glm::vec4& color)
	{
		Get()->m_primitives.fillCircle(center, radius, color, GetView());
	}

	void Renderer::FillPolygon(const std::vector<glm::vec2>& vertices, const glm::vec4& color)
	{
		Get()->m_primitives.fillPolygon(vertices, color, GetView());
	}

	glm::mat4 Renderer::GetView()
	{
		return Get()->m_view;
	}
}

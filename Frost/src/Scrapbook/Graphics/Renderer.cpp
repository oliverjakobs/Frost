#include "Renderer.h"

#include "ResourceManager.h"

namespace sb
{
	void Renderer::Init(float x, float y, float w, float h)
	{
		Get()->m_primitives.create();

		Get()->m_screenView = glm::ortho(0.0f, w, 0.0f, h);

		SetView(x, y, w, h);
	}

	void Renderer::Destroy()
	{

	}

	void Renderer::Start()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::Flush()
	{
		Get()->m_primitives.flush(GetView().mat);
	}

	void Renderer::EnableBlend(GLenum sfactor, GLenum dfactor)
	{
		glEnable(GL_BLEND);
		glBlendFunc(sfactor, dfactor);
	}

	void Renderer::SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void Renderer::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Renderer::RenderTexture(Texture* tex, const std::string& sName, const glm::mat4& model, const glm::mat4& view, const glm::mat4& perspective, const glm::vec2& framePos)
	{
		if (tex != nullptr)
		{
			shared_ptr<Shader> shader = ResourceManager::GetShader(sName);

			if (shader != nullptr)
			{
				shader->use();

				shader->setUniform2f("uFramePos", framePos);

				shader->setUniformMat4("projection", perspective);
				shader->setUniformMat4("view", view);
				shader->setUniformMat4("model", model);
			}

			tex->bind();

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}

	void Renderer::RenderTexture(Texture* tex, const std::string& sName, const glm::mat4& model, const glm::mat4& view, const glm::mat4& perspective, const std::vector<GLuint>& indices, const glm::vec2& framePos)
	{
		if (tex != nullptr)
		{
			shared_ptr<Shader> shader = ResourceManager::GetShader(sName);

			if (shader != nullptr)
			{
				shader->use();

				shader->setUniform2f("uFramePos", framePos);

				shader->setUniformMat4("projection", perspective);
				shader->setUniformMat4("view", view);
				shader->setUniformMat4("model", model);
			}

			tex->bind();

			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
		}
	}

	void Renderer::DrawLine(const Line& line, const glm::vec4& color, const glm::mat4& view)
	{
		Get()->m_primitives.drawLine(line.start, line.end, color, view);
	}

	void Renderer::DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color, const glm::mat4& view)
	{
		Get()->m_primitives.drawLine(start, end, color, view);
	}

	void Renderer::DrawRect(float x, float y, float w, float h, const glm::vec4& color, const glm::mat4& view)
	{
		std::vector<glm::vec2> vertices =
		{
			glm::vec2(x, y),
			glm::vec2(x + w, y),
			glm::vec2(x + w, y + h),
			glm::vec2(x, y + h)
		};

		Get()->m_primitives.drawPolygon(vertices, color, view);
	}

	void Renderer::DrawRect(const glm::vec2& pos, const glm::vec2& dim, const glm::vec4& color, const glm::mat4& view)
	{
		DrawRect(pos.x, pos.y, dim.x, dim.y, color, view);
	}

	void Renderer::DrawCircle(const glm::vec2& center, float radius, const glm::vec4& color, const glm::mat4& view)
	{
		Get()->m_primitives.drawCircle(center, radius, color, view);
	}

	void Renderer::DrawPolygon(const std::vector<glm::vec2>& vertices, const glm::vec4& color, const glm::mat4& view)
	{
		Get()->m_primitives.drawPolygon(vertices, color, view);
	}

	void Renderer::FillRect(float x, float y, float w, float h, const glm::vec4& color, const glm::mat4& view)
	{
		std::vector<glm::vec2> vertices =
		{
			glm::vec2(x, y),
			glm::vec2(x + w, y),
			glm::vec2(x + w, y + h),
			glm::vec2(x, y + h)
		};

		Get()->m_primitives.fillPolygon(vertices, color, view);
	}
	
	void Renderer::FillCircle(const glm::vec2& center, float radius, const glm::vec4& color, const glm::mat4& view)
	{
		Get()->m_primitives.fillCircle(center, radius, color, view);
	}

	void Renderer::FillPolygon(const std::vector<glm::vec2>& vertices, const glm::vec4& color, const glm::mat4& view)
	{
		Get()->m_primitives.fillPolygon(vertices, color, view);
	}

	void Renderer::SetView(float x, float y)
	{
		SetView(x, y, Get()->m_view.w, Get()->m_view.h);
	}

	void Renderer::SetView(float x, float y, float w, float h)
	{
		Get()->m_view.x = x;
		Get()->m_view.y = y;
		Get()->m_view.w = w;
		Get()->m_view.h = h;
		Get()->m_view.mat = glm::ortho(x, x + w, y, y + h);
	}

	void Renderer::SetViewCenter(float x, float y, Rect* con)
	{
		if (con != nullptr)
		{
			x -= GetView().w / 2.0f;
			y -= GetView().h / 2.0f;

			if (x < con->pos.x)
				x = con->pos.x;

			if (x + GetView().w > con->pos.x + con->dim.x)
				x = con->pos.x + con->dim.x - GetView().w;

			if (y < con->pos.y)
				y = con->pos.y;

			if (y + GetView().h > con->pos.y + con->dim.y)
				y = con->pos.y + con->dim.y - GetView().h;

			SetView(x, y);
		}
		else
		{
			SetView(x - GetView().w / 2.0f, y - GetView().h / 2.0f);
		}
	}

	View Renderer::GetView()
	{
		return Get()->m_view;
	}

	glm::mat4 Renderer::GetViewMat()
	{
		return GetView().mat;
	}

	glm::mat4 Renderer::GetScreenView()
	{
		return Get()->m_screenView;
	}
}

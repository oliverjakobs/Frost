#include "Renderer.h"

#include "ResourceManager.h"

void Renderer::Init(float x, float y, float w, float h)
{
	Get().m_primitives.create();

	Get().m_screenView = glm::ortho(0.0f, w, 0.0f, h);

	SetView(x, y, w, h);
}

void Renderer::Destroy()
{
	Get().m_primitives.destroy();
}

void Renderer::Start()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Flush()
{
	Get().m_primitives.flush(GetView().mat);
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

void Renderer::RenderTexture(Texture* tex)
{
	if (tex != nullptr)
	{
		tex->Bind();
		std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
	}
}

void Renderer::RenderTextureInstanced(Texture* tex, uint instanceCount)
{
	if (tex != nullptr)
	{
		tex->Bind();
		std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };

		glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data(), instanceCount);
	}
}

void Renderer::RenderString(Font* font, const std::string& text, float x, float y, const glm::mat4& view, const std::string& shader)
{
	font->onRender(text, x, y, view, shader);
}

void Renderer::DrawLine(const Line& line, const glm::vec4& color, const glm::mat4& view)
{
	Get().m_primitives.drawLine(line.start, line.end, color, view);
}

void Renderer::DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color, const glm::mat4& view)
{
	Get().m_primitives.drawLine(start, end, color, view);
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

	Get().m_primitives.drawPolygon(vertices, color, view);
}

void Renderer::DrawRect(const glm::vec2& pos, const glm::vec2& dim, const glm::vec4& color, const glm::mat4& view)
{
	DrawRect(pos.x, pos.y, dim.x, dim.y, color, view);
}

void Renderer::DrawCircle(const glm::vec2& center, float radius, const glm::vec4& color, const glm::mat4& view)
{
	Get().m_primitives.drawCircle(center, radius, color, view);
}

void Renderer::DrawPolygon(const std::vector<glm::vec2>& vertices, const glm::vec4& color, const glm::mat4& view)
{
	Get().m_primitives.drawPolygon(vertices, color, view);
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

	Get().m_primitives.fillPolygon(vertices, color, view);
}

void Renderer::FillCircle(const glm::vec2& center, float radius, const glm::vec4& color, const glm::mat4& view)
{
	Get().m_primitives.fillCircle(center, radius, color, view);
}

void Renderer::FillPolygon(const std::vector<glm::vec2>& vertices, const glm::vec4& color, const glm::mat4& view)
{
	Get().m_primitives.fillPolygon(vertices, color, view);
}

void Renderer::SetView(float x, float y)
{
	SetView(x, y, Get().m_view.w, Get().m_view.h);
}

void Renderer::SetView(float x, float y, float w, float h)
{
	Get().m_view.x = x;
	Get().m_view.y = y;
	Get().m_view.w = w;
	Get().m_view.h = h;
	Get().m_view.mat = glm::ortho(x, x + w, y, y + h);
}

void Renderer::SetViewCenter(float x, float y)
{
	SetView(x - GetView().w / 2.0f, y - GetView().h / 2.0f);
}

void Renderer::SetViewCenter(const glm::vec2& cen)
{
	SetViewCenter(cen.x, cen.y);
}

void Renderer::SetViewCenter(float x, float y, Rect con)
{
	x -= GetView().w / 2.0f;
	y -= GetView().h / 2.0f;

	if (x < con.pos.x)
		x = con.pos.x;

	if (x + GetView().w > con.pos.x + con.dim.x)
		x = con.pos.x + con.dim.x - GetView().w;

	if (y < con.pos.y)
		y = con.pos.y;

	if (y + GetView().h > con.pos.y + con.dim.y)
		y = con.pos.y + con.dim.y - GetView().h;

	SetView(x, y);
}

void Renderer::SetViewCenter(const glm::vec2& cen, Rect constraint)
{
	SetViewCenter(cen.x, cen.y, constraint);
}

View Renderer::GetView()
{
	return Get().m_view;
}

glm::mat4 Renderer::GetViewMat()
{
	return GetView().mat;
}

glm::mat4 Renderer::GetScreenView()
{
	return Get().m_screenView;
}

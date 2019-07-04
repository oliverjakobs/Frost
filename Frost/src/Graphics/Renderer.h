#pragma once

#include "ResourceManager.h"
#include "Primitives.h"

#include "Maths/Maths.h"

const Color BLACK =		Color(0.0f, 0.0f, 0.0f, 1.0f);
const Color WHITE =		Color(1.0f, 1.0f, 1.0f, 1.0f);
const Color RED =		Color(1.0f, 0.0f, 0.0f, 1.0f);
const Color GREEN =		Color(0.0f, 1.0f, 0.0f, 1.0f);
const Color BLUE =		Color(0.0f, 0.0f, 1.0f, 1.0f);
const Color YELLOW =	Color(1.0f, 1.0f, 0.0f, 1.0f);
const Color MAGENTA =	Color(1.0f, 0.0f, 1.0f, 1.0f);
const Color CYAN =		Color(0.0f, 1.0f, 1.0f, 1.0f);


inline Color blendColor(const Color& color, float alpha)
{
	return Color(color.r, color.g, color.b, alpha);
}

struct View
{
	float x;
	float y;
	float w;
	float h;

	glm::mat4 mat;

	inline glm::vec2 toWorldCoord(const glm::vec2& pos) const { return glm::vec2(pos.x, h - pos.y); }
};

class Renderer : private Singleton<Renderer>
{
private:
	View m_view;
	glm::mat4 m_screenView;

	Primitives m_primitives;
public:
	static void Init(float x, float y, float w, float h);
	static void Destroy();

	static void Start();
	static void Flush();

	static void EnableBlend(GLenum sfactor, GLenum dfactor);
	static void SetClearColor(float r, float g, float b, float a);
	static void SetClearColor(const Color& color);

	// ----------------------------texture---------------------------------------------------------------------------
	static void RenderTexture(Texture* tex);
	static void RenderTextureInstanced(Texture* tex, uint instanceCount);

	// ----------------------------primitives------------------------------------------------------------------------
	static void DrawLine(const Line& line, const Color& color, const glm::mat4& view = Renderer::GetViewMat());
	static void DrawLine(const glm::vec2& start, const glm::vec2& end, const Color& color, const glm::mat4& view = Renderer::GetViewMat());
	static void DrawRect(float x, float y, float w, float h, const Color& color, const glm::mat4& view = Renderer::GetViewMat());
	static void DrawRect(const glm::vec2& pos, const glm::vec2& dim, const Color& color, const glm::mat4& view = Renderer::GetViewMat());
	static void DrawCircle(const glm::vec2& center, float radius, const Color& color, const glm::mat4& view = Renderer::GetViewMat());
	static void DrawPolygon(const std::vector<glm::vec2>& vertices, const Color& color, const glm::mat4& view = Renderer::GetViewMat());

	static void FillRect(float x, float y, float w, float h, const Color& color, const glm::mat4& view = Renderer::GetViewMat());
	static void FillCircle(const glm::vec2& center, float radius, const Color& color, const glm::mat4& view = Renderer::GetViewMat());
	static void FillPolygon(const std::vector<glm::vec2>& vertices, const Color& color, const glm::mat4& view = Renderer::GetViewMat());

	// ----------------------------view------------------------------------------------------------------------------
	static void SetView(float x, float y);
	static void SetView(float x, float y, float width, float height);
	static void SetViewCenter(float x, float y);
	static void SetViewCenter(const glm::vec2& cen);
	static void SetViewCenter(float x, float y, Rect constraint);
	static void SetViewCenter(const glm::vec2& cen, Rect constraint);

	static View GetView();
	static glm::mat4 GetViewMat();
	static glm::mat4 GetScreenView();
};
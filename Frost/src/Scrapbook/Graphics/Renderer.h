#pragma once

#include <glad/glad.h>

#include "Texture.h"
#include "Shader.h"

#include "Primitives.h"

namespace sb
{
	const glm::vec4 BLACK = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	const glm::vec4 WHITE = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	const glm::vec4 RED = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	const glm::vec4 GREEN = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	const glm::vec4 BLUE = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	const glm::vec4 YELLOW = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	const glm::vec4 MAGENTA = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	const glm::vec4 CYAN = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);


	inline glm::vec4 blendColor(const glm::vec4& color, float alpha)
	{
		return glm::vec4(color.r, color.g, color.b, alpha);
	}

	struct View
	{
		float x;
		float y;
		float w;
		float h;

		glm::mat4 mat;
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
		static void SetClearColor(const glm::vec4& color);

		// ----------------------------texture---------------------------------------------------------------------------
		static void RenderTexture(Texture* tex, const std::string& shader, const glm::mat4& model, const glm::mat4& view, const glm::mat4& perspective, const glm::vec2& framePos = glm::vec2());
		static void RenderTexture(Texture* tex, const std::string& shader, const glm::mat4& model, const glm::mat4& view, const glm::mat4& perspective, const std::vector<GLuint>& indices, const glm::vec2& framePos = glm::vec2());

		// ----------------------------primitives------------------------------------------------------------------------
		static void DrawLine(const Line& line, const glm::vec4& color, const glm::mat4& view = Renderer::GetViewMat());
		static void DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color, const glm::mat4& view = Renderer::GetViewMat());
		static void DrawRect(float x, float y, float w, float h, const glm::vec4& color, const glm::mat4& view = Renderer::GetViewMat());
		static void DrawRect(const glm::vec2& pos, const glm::vec2& dim, const glm::vec4& color, const glm::mat4& view = Renderer::GetViewMat());
		static void DrawCircle(const glm::vec2& center, float radius, const glm::vec4& color, const glm::mat4& view = Renderer::GetViewMat());
		static void DrawPolygon(const std::vector<glm::vec2>& vertices, const glm::vec4& color, const glm::mat4& view = Renderer::GetViewMat());

		static void FillRect(float x, float y, float w, float h, const glm::vec4& color, const glm::mat4& view = Renderer::GetViewMat());
		static void FillCircle(const glm::vec2& center, float radius, const glm::vec4& color, const glm::mat4& view = Renderer::GetViewMat());
		static void FillPolygon(const std::vector<glm::vec2>& vertices, const glm::vec4& color, const glm::mat4& view = Renderer::GetViewMat());

		// ----------------------------view------------------------------------------------------------------------------
		static void SetView(float x, float y);
		static void SetView(float x, float y, float width, float height);
		static void SetViewCenter(float x, float y, Rect* constraint);

		static View GetView();
		static glm::mat4 GetViewMat();
		static glm::mat4 GetScreenView();
	};
}
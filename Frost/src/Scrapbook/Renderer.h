#pragma once

#include <glad\glad.h>

#include "Util\Singelton.h"
#include "Texture.h"
#include "Shader.h"

#include "Primitives.h"

#include "Math.h"

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

	class Renderer : private Singleton<Renderer>
	{
	private:
		glm::mat4 m_view;

		Primitives m_primitives;
	public:
		static void Init(float x, float y, float w, float h);
		
		static void Start();
		static void Flush();

		static void EnableBlend(GLenum sfactor, GLenum dfactor);

		// ----------------------------texture---------------------------------------------------------------------------
		static void RenderTexture(Texture* tex, const glm::vec2& position, Shader* shader = nullptr);

		// ----------------------------primitives------------------------------------------------------------------------
		static void DrawLine(const Line& line, const glm::vec4& color);
		static void DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color);
		static void DrawRect(float x, float y, float w, float h, const glm::vec4& color);
		static void DrawRect(const glm::vec2& pos, const glm::vec2& dim, const glm::vec4& color);
		static void DrawCircle(const glm::vec2& center, float radius, const glm::vec4& color);
		static void DrawPolygon(const std::vector<glm::vec2>& vertices, const glm::vec4& color);

		static void FillRect(float x, float y, float w, float h, const glm::vec4& color);
		static void FillCircle(const glm::vec2& center, float radius, const glm::vec4& color);
		static void FillPolygon(const std::vector<glm::vec2>& vertices, const glm::vec4& color);

		static glm::mat4 GetView();
	};
}
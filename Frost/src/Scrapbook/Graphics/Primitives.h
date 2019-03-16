#pragma once

#include "Maths/Maths.h"
#include "Scrapbook/Utility.h"

namespace sb
{
	struct GLRenderLines;
	struct GLRenderTriangles;

	class Primitives
	{
	private:
		GLRenderLines* m_lines;
		GLRenderTriangles* m_triangles;

	public:
		Primitives();
		~Primitives();

		void create();
		void destroy();

		void flush(const glm::mat4& view);

		void drawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color, const glm::mat4& view) const;
		void drawPolygon(const std::vector<glm::vec2>& vertices, const glm::vec4& color, const glm::mat4& view) const;
		void drawCircle(const glm::vec2& center, float radius, const glm::vec4& color, const glm::mat4& view) const;

		void fillPolygon(const std::vector<glm::vec2>& vertices, const glm::vec4& color, const glm::mat4& view) const;
		void fillCircle(const glm::vec2& center, float radius, const glm::vec4& color, const glm::mat4& view) const;
	};
}
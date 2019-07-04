#pragma once

#include "Types.h"

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

	void drawLine(const glm::vec2& start, const glm::vec2& end, const Color& color, const glm::mat4& view) const;
	void drawPolygon(const Vertices& vertices, const Color& color, const glm::mat4& view) const;
	void drawCircle(const glm::vec2& center, float radius, const Color& color, const glm::mat4& view) const;

	void fillPolygon(const Vertices& vertices, const Color& color, const glm::mat4& view) const;
	void fillCircle(const glm::vec2& center, float radius, const Color& color, const glm::mat4& view) const;
};
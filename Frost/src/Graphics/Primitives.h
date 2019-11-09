#pragma once

#include "Types.h"
#include "Maths/Maths.h"
#include "Obelisk/Singelton.h"

struct GLRenderLines;
struct GLRenderTriangles;

class Primitives : private Singleton<Primitives>
{
private:
	GLRenderLines* m_lines;
	GLRenderTriangles* m_triangles;

public:
	Primitives();
	~Primitives();

	static void Flush(const glm::mat4& view);

	// Line
	static void DrawLine(const Line& line, const ignis::color& color, const glm::mat4& view);
	static void DrawLine(const glm::vec2& start, const glm::vec2& end, const ignis::color& color, const glm::mat4& view);

	// Rect
	static void DrawRect(float x, float y, float w, float h, const ignis::color& color, const glm::mat4& view);
	static void DrawRect(const glm::vec2& pos, const glm::vec2& dim, const ignis::color& color, const glm::mat4& view);
	static void FillRect(float x, float y, float w, float h, const ignis::color& color, const glm::mat4& view);
	static void FillRect(const glm::vec2& pos, const glm::vec2& dim, const ignis::color& color, const glm::mat4& view);

	// Polygon
	static void DrawPolygon(const Vertices& vertices, const ignis::color& color, const glm::mat4& view);
	static void FillPolygon(const Vertices& vertices, const ignis::color& color, const glm::mat4& view);

	// Circle
	static void DrawCircle(const glm::vec2& center, float radius, const ignis::color& color, const glm::mat4& view);
	static void FillCircle(const glm::vec2& center, float radius, const ignis::color& color, const glm::mat4& view);
};
#pragma once

#include "Ignis/Ignis.h"

#include <glm/glm.hpp>
#include <vector>

struct Primitives2D
{
	static void Init(ignis_shader* shader);
	static void Destroy();

	static void Start(const glm::mat4& viewProjection);
	static void Flush();

	static void Vertex(const glm::vec2& v, const ignis_color_rgba& c);

	// Line
	static void DrawLine(float x1, float y1, float x2, float y2, const ignis_color_rgba& c);
	static void DrawLine(const glm::vec2& start, const glm::vec2& end, const ignis_color_rgba& c);

	// Rect
	static void DrawRect(float x, float y, float w, float h, const ignis_color_rgba& c);
	static void DrawRect(const glm::vec2& pos, const glm::vec2& dim, const ignis_color_rgba& c);

	// Polygon
	static void DrawPolygon(const std::vector<glm::vec2>& vertices, const ignis_color_rgba& c);

	// Circle
	static void DrawCircle(const glm::vec2& center, float radius, const ignis_color_rgba& c);
};
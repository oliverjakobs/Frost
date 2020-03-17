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

	static void Vertex(float x, float y, const ignis_color_rgba& c);

	static void DrawLine(float x1, float y1, float x2, float y2, const ignis_color_rgba& c);
	static void DrawRect(float x, float y, float w, float h, const ignis_color_rgba& c);
	static void DrawPolygon(float* vertices, size_t count, const ignis_color_rgba& c);
	static void DrawCircle(float x, float y, float radius, const ignis_color_rgba& c);
};
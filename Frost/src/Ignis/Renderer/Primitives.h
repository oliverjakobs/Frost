#pragma once

#include "Ignis/Core/Buffer.h"
#include "Ignis/Core/Shader.h"

namespace ignis
{
	struct Primitives2D
	{
		static void Init(const std::shared_ptr<Shader>& shader);
		static void Destroy();

		static void Start(const glm::mat4& viewProjection);
		static void Flush();

		static void Vertex(const glm::vec2& v);

		// Line
		static void DrawLine(const glm::vec2& start, const glm::vec2& end);

		// Rect
		static void DrawRect(float x, float y, float w, float h);
		static void DrawRect(const glm::vec2& pos, const glm::vec2& dim);

		// Polygon
		static void DrawPolygon(const std::vector<glm::vec2>& vertices);

		// Circle
		static void DrawCircle(const glm::vec2& center, float radius);
	};
}
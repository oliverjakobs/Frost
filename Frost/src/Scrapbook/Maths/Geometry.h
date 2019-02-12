#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

namespace sb
{
	inline float distance(const glm::vec2& a, const glm::vec2& b)
	{
		return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
	}

	struct Line
	{
		glm::vec2 start;
		glm::vec2 end;

		Line() : start(glm::vec2()), end(glm::vec2()) {}
		Line(const glm::vec2& s, const glm::vec2& e) : start(s), end(e) {}
		Line(float x1, float y1, float x2, float y2) : start(glm::vec2(x1, y1)), end(glm::vec2(x2, y2)) {}
	};

	struct Rect
	{
		glm::vec2 pos;
		glm::vec2 dim;

		Rect() : pos(glm::vec2()), dim(glm::vec2()) {}
		Rect(float x, float y, float w, float h) : pos(glm::vec2(x,y)), dim(glm::vec2(w,h)) {}
		Rect(glm::vec2 pos, glm::vec2 dim) : pos(pos), dim(dim) {}
	};
}
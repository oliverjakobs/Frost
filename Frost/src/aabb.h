#pragma once

#include <glm\glm.hpp>

struct Line
{
	glm::vec2 start;
	glm::vec2 end;

	Line() : start(glm::vec2()), end(glm::vec2()) {}
	Line(const glm::vec2& s, const glm::vec2& e) : start(s), end(e) {}
	Line(float x1, float y1, float x2, float y2) : start(glm::vec2(x1,y1)), end(glm::vec2(x2,y2)) {}
};

struct AABB
{
	glm::vec2 center;
	glm::vec2 halfDimension;

	AABB() : center(glm::vec2()), halfDimension(glm::vec2()) {}
	AABB(const glm::vec2& cen, const glm::vec2& halfDim) : center(cen), halfDimension(halfDim) {}
};
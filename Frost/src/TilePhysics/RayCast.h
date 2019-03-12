#pragma once

#include "Scrapbook/Maths.h"

using namespace sb;

struct Intersection
{
	glm::vec2 pos;
	Line edge;
};

typedef std::vector<Intersection> RayCastResult;

// based on an algorithm in Andre LeMothe's "Tricks of the Windows Game Programming Gurus"
bool LineIntersection(const Line& line1, const Line& line2, glm::vec2* out)
{
	float s10_x = line1.end.x - line1.start.x;
	float s10_y = line1.end.y - line1.start.y;
	float s32_x = line2.end.x - line2.start.x;
	float s32_y = line2.end.y - line2.start.y;

	float denom = s10_x * s32_y - s32_x * s10_y;

	if (denom == 0) return false; // Collinear

	bool denomPositive = denom > 0;

	float s02_x = line1.start.x - line2.start.x;
	float s02_y = line1.start.y - line2.start.y;

	float s_numer = s10_x * s02_y - s10_y * s02_x;
	float t_numer = s32_x * s02_y - s32_y * s02_x;

	if ((s_numer < 0) == denomPositive || (t_numer < 0) == denomPositive) return false; // No collision

	if (((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive)) return false; // No collision

																									// Collision detected
	float t = t_numer / denom;

	if (out != nullptr)
		*out = glm::vec2(line1.start.x + (t * s10_x), line1.start.y + (t * s10_y));

	return true;
}

// returns all intersections with the edges along the ray (sorted by the distance to the start of the ray)
RayCastResult RayCast(const Line& ray, std::vector<Line> edges)
{
	RayCastResult result;

	for (auto& e : edges)
	{
		glm::vec2 interSec;

		if (LineIntersection(ray, e, &interSec))
			result.push_back({ interSec, e });
	}

	if (result.empty())
		return result;

	std::sort(result.begin(), result.end(), [&](const auto& p1, const auto& p2)
	{
		return glm::distance(ray.start, p1.pos) < glm::distance(ray.start, p2.pos);
	});

	return result;
}

RayCastResult CircularRayCast(const glm::vec2& pos, std::vector<Line> edges, float count)
{
	RayCastResult result;

	const float increment = 2.0f * M_PI / count;

	float sinInc = sinf(increment);
	float cosInc = cosf(increment);

	glm::vec2 r1(1.0f, 0.0f);
	glm::vec2 v = pos + 1000.0f * r1;

	for (int i = 0; i < count; ++i)
	{
		// Perform rotation to avoid additional trigonometry.
		glm::vec2 r2;
		r2.x = cosInc * r1.x - sinInc * r1.y;
		r2.y = sinInc * r1.x + cosInc * r1.y;

		v = pos + 1000.0f * r2;

		Line ray = Line(pos, v);

		auto inter = RayCast(ray, edges);

		if (!inter.empty())
			result.push_back(inter.front());
		else
			result.push_back({ ray.end, Line() });

		r1 = r2;
	}

	return result;
}

RayCastResult SmartRayCast(const glm::vec2& pos, std::vector<Line> edges)
{
	RayCastResult result;


	return result;
}
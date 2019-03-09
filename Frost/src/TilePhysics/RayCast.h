#pragma once

#include "Scrapbook/Maths.h"

using namespace sb;

struct Intersection
{
	glm::vec2 pos;
	Line edge;
};

bool LineIntersection(const Line& line1, const Line& line2, glm::vec2* out);

Intersection* GetIntersection(const Line& ray, std::vector<Line> edges)
{
	std::vector<Intersection> intersections;

	for (auto& e : edges)
	{
		glm::vec2 interSec;

		if (LineIntersection(ray, e, &interSec))
			intersections.push_back({ interSec, e });
	}

	if (intersections.empty())
		return nullptr;

	std::sort(intersections.begin(), intersections.end(), [&](const auto& p1, const auto& p2)
	{
		return glm::distance(ray.start, p1.pos) < glm::distance(ray.start, p2.pos);
	});

	return &intersections[0];
}

void RayCast(const glm::vec2& pos, std::vector<Line> edges, float count)
{
	const float k_increment = 2.0f * M_PI / count;

	float sinInc = sinf(k_increment);
	float cosInc = cosf(k_increment);

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

		Intersection* in = GetIntersection(ray, edges);

		if (in != nullptr)
		{
			Renderer::FillCircle(in->pos, 4, BLACK);
			Renderer::DrawLine(in->edge, BLUE);
			Renderer::DrawLine(pos, in->pos, RED);
		}
		else
			Renderer::DrawLine(pos, v, RED);

		r1 = r2;
	}
}

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
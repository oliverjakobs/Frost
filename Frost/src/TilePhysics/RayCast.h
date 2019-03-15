#pragma once

#include "Scrapbook/Maths.h"

#include <tuple>

using namespace sb;

struct Intersection
{
	glm::vec2 pos;
	Line edge;
};

bool cmp_float(float a, float b, float epsilon = 0.000001f)
{
	return (fabs(a - b) < epsilon);
}

bool less_float(float a, float b, float epsilon = 0.000001f) 
{
	if (!cmp_float(a, b, epsilon)) // if a < b then a != b
		return a < b;

	return false;
}

bool greater_float(float a, float b, float epsilon = 0.000001f)
{
	if (!cmp_float(a, b, epsilon)) // if a > b then a != b
		return a > b;

	return false;
}

typedef std::vector<Intersection> RayCastResult;

// based on an algorithm in Andre LeMothe's "Tricks of the Windows Game Programming Gurus"
bool LineIntersection(const Line& line1, const Line& line2, glm::vec2* out)
{
	float dl1_x = line1.end.x - line1.start.x;
	float dl1_y = line1.end.y - line1.start.y;
	float dl2_x = line2.end.x - line2.start.x;
	float dl2_y = line2.end.y - line2.start.y;

	float denom = dl1_x * dl2_y - dl2_x * dl1_y;

	if (denom == 0) return false; // Collinear

	bool denomPositive = denom > 0;

	float s02_x = line1.start.x - line2.start.x;
	float s02_y = line1.start.y - line2.start.y;

	float s_numer = dl1_x * s02_y - dl1_y * s02_x;
	float t_numer = dl2_x * s02_y - dl2_y * s02_x;

	if ((s_numer < 0) == denomPositive || (t_numer < 0) == denomPositive) return false; // No collision

	if (((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive)) return false; // No collision

	// Collision detected
	float t = t_numer / denom;

	if (out != nullptr)
		*out = glm::vec2(line1.start.x + (t * dl1_x), line1.start.y + (t * dl1_y));

	return true;
}

std::vector<std::tuple<float, float, float>> getVisibilityPolygonPoints(const glm::vec2& pos, std::vector<Line> edges, float range)
{
	std::vector<std::tuple<float, float, float>> vertices;

	// For each edge in PolyMap
	for (auto &e1 : edges)
	{
		// Take the start point, then the end point (we could use a pool of
		// non-duplicated points here, it would be more optimal)
		for (int i = 0; i < 2; i++)
		{
			float rdx, rdy;
			rdx = (i == 0 ? e1.start.x : e1.end.x) - pos.x;
			rdy = (i == 0 ? e1.start.y : e1.end.y) - pos.y;

			float base_ang = atan2f(rdy, rdx);

			float ang = 0;
			// For each point, cast 3 rays, 1 directly at point
			// and 1 a little bit either side
			for (int j = 0; j < 3; j++)
			{
				if (j == 0)	ang = base_ang - 0.0001f;
				if (j == 1)	ang = base_ang;
				if (j == 2)	ang = base_ang + 0.0001f;

				// Create ray along angle for required distance
				rdx = range * cosf(ang);
				rdy = range * sinf(ang);

				float min_t1 = range;
				float min_px = 0, min_py = 0, min_ang = 0;
				bool bValid = false;

				// Check for ray intersection with all edges
				for (auto &e2 : edges)
				{
					// Create line segment vector
					float sdx = e2.end.x - e2.start.x;
					float sdy = e2.end.y - e2.start.y;

					if (fabs(sdx - rdx) > 0.0f && fabs(sdy - rdy) > 0.0f)
					{
						// t2 is normalised distance from line segment start to line segment end of intersect point
						float t2 = (rdx * (e2.start.y - pos.y) + (rdy * (pos.x - e2.start.x))) / (sdx * rdy - sdy * rdx);
						// t1 is normalised distance from source along ray to ray length of intersect point
						float t1 = (e2.start.x + sdx * t2 - pos.x) / rdx;

						// If intersect point exists along ray, and along line 
						// segment then intersect point is valid
						if (t1 > 0 && t2 >= 0 && t2 <= 1.0f)
						{
							// Check if this intersect point is closest to source. If
							// it is, then store this point and reject others
							if (t1 < min_t1)
							{
								min_t1 = t1;
								min_px = pos.x + rdx * t1;
								min_py = pos.y + rdy * t1;
								min_ang = atan2f(min_py - pos.y, min_px - pos.x);
								bValid = true;
							}
						}
					}
				}

				if (bValid)// Add intersection point to visibility polygon perimeter
					vertices.push_back({ min_ang, min_px, min_py });
			}
		}
	}

	// Sort perimeter points by angle from source. This will allow
	// us to draw a triangle fan.
	std::sort(vertices.begin(), vertices.end(),	[&](const auto& t1, const auto& t2)
	{
		return std::get<0>(t1) < std::get<0>(t2);
	});

	return vertices;
}

// returns all intersections with the edges along the ray (sorted by the distance to the start of the ray)
RayCastResult RayCast(const glm::vec2& pos, const glm::vec2& dir, std::vector<Line> edges, float rayMaxLength)
{
	RayCastResult result;

	for (auto& e : edges)
	{
		glm::vec2 interSec;

		if (LineIntersection(Line(pos, pos + rayMaxLength * dir), e, &interSec))
			result.push_back({ interSec, e });
	}

	if (result.empty())
		return result;

	std::sort(result.begin(), result.end(), [&](const auto& p1, const auto& p2)
	{
		return glm::distance(pos, p1.pos) < glm::distance(pos, p2.pos);
	});

	return result;
}

RayCastResult CircularRayCast(const glm::vec2& pos, std::vector<Line> edges, float count)
{
	RayCastResult result;

	const float increment = 2.0f * M_PI / count;

	float sinInc = sinf(increment);
	float cosInc = cosf(increment);

	glm::vec2 oldDir(1.0f, 0.0f);

	for (int i = 0; i < count; ++i)
	{
		// Perform rotation to avoid additional trigonometry.
		glm::vec2 dir;
		dir.x = cosInc * oldDir.x - sinInc * oldDir.y;
		dir.y = sinInc * oldDir.x + cosInc * oldDir.y;
		
		auto inter = RayCast(pos, dir, edges, 1000.0f);

		if (!inter.empty())
			result.push_back(inter.front());

		oldDir = dir;
	}

	return result;
}
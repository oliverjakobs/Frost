#include "Visibility.h"

#include <stdlib.h>

vec3* Visibility(vec2 pos, line* edges, size_t edge_count, size_t* intersection_count)
{
	vec3* intersections = (vec3*)malloc(sizeof(vec3) * 3 * 2 * edge_count);
	size_t index = 0;

	for (size_t i = 0; i < edge_count; ++i)
	{
		for (size_t e = 0; e < 2; e++)
		{
			vec2  v;

			if (e == 0)	v = edges[i].start;
			if (e == 1)	v = edges[i].end;

			vec2 rd = vec2_sub(v, pos);

			float base_ang = atan2f(rd.y, rd.x);

			float ang = 0;

			// For each point, cast 3 rays, 1 directly at point and 1 a little bit either side
			for (int ray = 0; ray < 3; ++ray)
			{
				if (ray == 0)	ang = base_ang - 0.0001f;
				if (ray == 1)	ang = base_ang;
				if (ray == 2)	ang = base_ang + 0.0001f;

				// Create ray along angle for required distance
				rd = (vec2){ cosf(ang), sinf(ang) };

				float min_t1 = INFINITY;
				vec2 minPos = (vec2){ 0.0f, 0.0f };
				float minAng = 0;
				int valid = 0;

				// Check for ray intersection with all edges
				for (size_t j = 0; j < edge_count; ++j)
				{
					// Create line segment vector
					vec2 sd = vec2_sub(edges[j].end, edges[j].start);

					if (fabs(sd.x - rd.x) > 0.0f && fabs(sd.y - rd.y) > 0.0f)
					{
						// t2 is normalised distance from line segment start to line segment end of intersect point
						float t2 = (rd.x * (edges[j].start.y - pos.y) + (rd.y * (pos.x - edges[j].start.x))) / (sd.x * rd.y - sd.y * rd.x);
						// t1 is normalised distance from source along ray to ray length of intersect point
						float t1 = (edges[j].start.x + sd.x * t2 - pos.x) / rd.x;

						// If intersect point exists along ray, and along line 
						// segment then intersect point is valid
						if (t1 > 0 && t2 >= 0 && t2 <= 1.0f)
						{
							// Check if this intersect point is closest to source. If
							// it is, then store this point and reject others
							if (t1 < min_t1)
							{
								min_t1 = t1;
								minPos = vec2_add(pos, vec2_mult(rd, t1));
								minAng = atan((minPos.y - pos.y) / (minPos.x - pos.x));
								valid = 1;
							}
						}
					}
				}

				if (valid)// Add intersection point to visibility polygon perimeter
					intersections[index++] = (vec3){ minPos.x, minPos.y, minAng };
			}
		}
	}

	*intersection_count = index;

	// std::sort(intersections.begin(), intersections.end(), [&](const auto& t1, const auto& t2)
	// 	{
	// 		return t1.z < t2.z;
	// 	});

	return intersections;
}

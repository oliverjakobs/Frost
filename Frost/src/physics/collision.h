#ifndef COLLISION_H
#define COLLISION_H

#include "math/math.h"

typedef struct
{
	vec2 min;
	vec2 max;
} aabb;

typedef struct
{
	vec2 pos;
	vec2 normal;
	float t_hit;
} contact;

int collision_ray_rect(vec2 ray_origin, vec2 ray_dir, rect target, vec2* contact_point, vec2* contact_normal, float* t_hit_near);

#endif /* !COLLISION_H */


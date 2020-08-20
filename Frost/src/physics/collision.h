#ifndef COLLISION_H
#define COLLISION_H

#include "math/math.h"

typedef struct
{
	vec2 pos;
	vec2 normal;
	float t;
} contact_manifold;

typedef struct
{
	vec2 min;
	vec2 max;
	int i;
	contact_manifold c;
} aabb;

void aabb_set_position(aabb* body, vec2 pos);
void aabb_set_center(aabb* body, vec2 center);

vec2 aabb_get_center(const aabb* body);
vec2 aabb_get_size(const aabb* body);

void aabb_move(aabb* body, vec2 move);

aabb aabb_expand(const aabb* base, vec2 expand);

int aabb_overlap(aabb a, aabb b);

int collision_ray_rect(vec2 ray_origin, vec2 ray_dir, aabb target, contact_manifold* contact);

int collision_aabb(aabb rect, aabb target, vec2 velocity, contact_manifold* contact);

#endif /* !COLLISION_H */


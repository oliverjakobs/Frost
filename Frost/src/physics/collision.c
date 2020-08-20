#include "collision.h"

#include "clib/clib.h"

void aabb_set_position(aabb* body, vec2 pos)
{
	body->min = pos;
	body->max = vec2_add(pos, aabb_get_size(body));
}

void aabb_set_center(aabb* body, vec2 center)
{
	vec2 half_size = vec2_div(aabb_get_size(body), 2.0f);

	body->min = vec2_sub(center, half_size);
	body->max = vec2_add(center, half_size);
}

vec2 aabb_get_center(const aabb* body)
{
	vec2 half_size = vec2_div(aabb_get_size(body), 2.0f);

	return vec2_add(body->min, half_size);
}

vec2 aabb_get_size(const aabb* body)
{
	return vec2_sub(body->max, body->min);
}

void aabb_move(aabb* body, vec2 move)
{
	body->min = vec2_add(body->min, move);
	body->max = vec2_add(body->max, move);
}

aabb aabb_expand(const aabb* base, vec2 expand)
{
	aabb expanded;
	expanded.min.x = base->min.x - expand.x;
	expanded.min.y = base->min.y - expand.y;
	expanded.max.x = base->max.x + expand.x;
	expanded.max.y = base->max.y + expand.y;

	return expanded;
}

int aabb_overlap(aabb a, aabb b)
{
	return !(b.max.x < a.min.x || a.max.x < b.min.x || b.max.y < a.min.y || a.max.y < b.min.y);
}

int collision_ray_rect(vec2 ray_origin, vec2 ray_dir, aabb target, contact_manifold* contact)
{
	contact->pos = vec2_zero();
	contact->normal = vec2_zero();

	float inv_dir_x = 1.0f / ray_dir.x;
	float inv_dir_y = 1.0f / ray_dir.y;

	float t_near_x = (target.min.x - ray_origin.x) * inv_dir_x;
	float t_near_y = (target.min.y - ray_origin.y) * inv_dir_y;
	float t_far_x = (target.max.x - ray_origin.x) * inv_dir_x;
	float t_far_y = (target.max.y - ray_origin.y) * inv_dir_y;

	if (t_near_x > t_far_x) clib_swapf(&t_near_x, &t_far_x);
	if (t_near_y > t_far_y) clib_swapf(&t_near_y, &t_far_y);

	if (t_near_x > t_far_y || t_near_y > t_far_x) return 0;

	float t_hit_near = MAX(t_near_x, t_near_y);
	float t_hit_far = MIN(t_far_x, t_far_y);

	if (t_hit_far < 0) return 0;

	contact->t = t_hit_near;

	contact->pos.x = ray_origin.x + t_hit_near * ray_dir.x;
	contact->pos.y = ray_origin.y + t_hit_near * ray_dir.y;

	if (t_near_x > t_near_y)
		contact->normal.x = (ray_dir.x < 0) ? 1.0f : -1.0f;
	else if (t_near_x < t_near_y)
		contact->normal.y = (ray_dir.y < 0) ? 1.0f : -1.0f;

	return contact->t <= 1.0f;
}

int collision_aabb(aabb rect, aabb target, vec2 velocity, contact_manifold* contact)
{
	if (velocity.x == 0 && velocity.y == 0)
		return 0;

	float half_w = (rect.max.x - rect.min.x) / 2.0f;
	float half_h = (rect.max.y - rect.min.y) / 2.0f;

	aabb expanded_target;
	expanded_target.min.x = target.min.x - half_w;
	expanded_target.min.y = target.min.y - half_h;
	expanded_target.max.x = target.max.x + half_w;
	expanded_target.max.y = target.max.y + half_h;

	vec2 rect_center;
	rect_center.x = rect.min.x + half_w;
	rect_center.y = rect.min.y + half_h;

	if (collision_ray_rect(rect_center, velocity, expanded_target, contact))
	{
		if (contact->t <= 1.0f)
		{
			return 1;
		}
	}

	return 0;
}

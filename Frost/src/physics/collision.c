#include "collision.h"

#include "clib/clib.h"

int collision_ray_rect(vec2 ray_origin, vec2 ray_dir, rect target, vec2* contact_point, vec2* contact_normal, float* t_hit_near)
{
	float t_near_x = (target.min.x - ray_origin.x) / ray_dir.x;
	float t_near_y = (target.min.y - ray_origin.y) / ray_dir.y;
	float t_far_x = (target.max.x - ray_origin.x) / ray_dir.x;
	float t_far_y = (target.max.y - ray_origin.y) / ray_dir.y;

	if (t_near_x > t_far_x) clib_swapf(&t_near_x, &t_far_x);
	if (t_near_y > t_far_y) clib_swapf(&t_near_y, &t_far_y);

	if (t_near_x > t_far_y || t_near_y > t_far_x) return 0;

	*t_hit_near = MAX(t_near_x, t_near_y);
	float t_hit_far = MIN(t_far_x, t_far_y);

	if (t_hit_far < 0) return 0;

	contact_point->x = ray_origin.x + *t_hit_near * ray_dir.x;
	contact_point->y = ray_origin.y + *t_hit_near * ray_dir.y;

	contact_normal->x = 0.0f;
	contact_normal->y = 0.0f;

	if (t_near_x > t_near_y)
		contact_normal->x = (ray_dir.x < 0) ? 1.0f : -1.0f;
	else if (t_near_x < t_near_y)
		contact_normal->y = (ray_dir.y < 0) ? 1.0f : -1.0f;

	return 1;
}

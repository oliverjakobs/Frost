#include "vec2.h"

vec2 vec2_(float x, float y)
{
	vec2 v;
	v.x = x;
	v.y = y;

	return v;
}

vec2 vec2_mult(const vec2 vec, float f)
{
	vec2 v;
	v.x = vec.x * f;
	v.y = vec.y * f;

	return v;
}

vec2 vec2_add(const vec2 a, const vec2 b)
{
	vec2 v;
	v.x = a.x + b.x;
	v.y = a.y + b.y;

	return v;
}

vec2 vec2_sub(const vec2 a, const vec2 b)
{
	vec2 v;
	v.x = a.x - b.x;
	v.y = a.y - b.y;

	return v;
}

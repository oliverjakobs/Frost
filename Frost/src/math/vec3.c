#include "vec3.h"

vec3 vec3_(float x, float y, float z)
{
	vec3 v;
	v.x = x;
	v.y = y;
	v.z = z;

	return v;
}

vec3 vec3_mult(const vec3 vec, float f)
{
	vec3 v;
	v.x = vec.x * f;
	v.y = vec.y * f;
	v.z = vec.z * f;

	return v;
}

vec3 vec3_add(const vec3 a, const vec3 b)
{
	vec3 v;
	v.x = a.x + b.x;
	v.y = a.y + b.y;
	v.z = a.z + b.z;

	return v;
}

vec3 vec3_sub(const vec3 a, const vec3 b)
{
	vec3 v;
	v.x = a.x - b.x;
	v.y = a.y - b.y;
	v.z = a.z - b.z;

	return v;
}

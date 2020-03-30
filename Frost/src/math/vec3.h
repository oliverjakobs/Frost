#ifndef VEC3_H
#define VEC3_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
	float x;
	float y;
	float z;
} vec3;

vec3 vec3_(float x, float y, float z);

vec3 vec3_mult(const vec3 v, float f);
vec3 vec3_add(const vec3 a, const vec3 b);
vec3 vec3_sub(const vec3 a, const vec3 b);

#ifdef __cplusplus
}
#endif

#endif /* !VEC3_H */

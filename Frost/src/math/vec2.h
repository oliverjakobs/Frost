#ifndef VEC2_H
#define VEC2_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
	float x;
	float y;
} vec2;

vec2 vec2_(float x, float y);

vec2 vec2_mult(const vec2 v, float f);
vec2 vec2_add(const vec2 a, const vec2 b);
vec2 vec2_sub(const vec2 a, const vec2 b);

#ifdef __cplusplus
}
#endif

#endif /* !VEC2_H */

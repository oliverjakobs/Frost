#ifndef VEC2_H
#define VEC2_H

typedef struct
{
	float x;
	float y;
} vec2;

vec2 vec2_mult(vec2 v, float f);
vec2 vec2_add(vec2 a, vec2 b);
vec2 vec2_sub(vec2 a, vec2 b);

int vec2_inside(vec2 point, vec2 min, vec2 max);

#endif /* !VEC2_H */

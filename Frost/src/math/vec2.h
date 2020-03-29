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

vec2 vec_2(float x, float y);

#ifdef __cplusplus
}
#endif

#endif // !VEC2_H

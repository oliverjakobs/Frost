#ifndef MAT4_H
#define MAT4_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "vec3.h"

typedef struct
{
	float values[4][4];
} mat4;

mat4 mat4_indentity();

mat4 mat4_translate(const mat4 m, vec3 v);
mat4 mat4_inverse(const mat4 m);
mat4 mat4_ortho(float left, float right, float bottom, float top, float near, float far);


mat4 mat4_scale(const mat4 m, float s);
mat4 mat4_multiply(const mat4 a, const mat4 b);

#ifdef __cplusplus
}
#endif

#endif /* !MAT4_H */

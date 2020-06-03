#ifndef MATH_H
#define MATH_H

#include <math.h>

/* based on https://github.com/felselva/mathc */
#include "vec2.h"
#include "vec3.h"

#include "mat4.h"

typedef struct
{
	float x;
	float y;
	float w;
	float h;
} rect;

typedef struct
{
	vec2 start;
	vec2 end;
} line;

#endif /* !MATH_H */

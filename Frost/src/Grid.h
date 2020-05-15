#ifndef GRID_H
#define GRID_H

#include "math/math.h"
#include "Graphics/Primitives2D.h"

float GridClip(float granularity, float f);
float GridClipDown(float granularity, float f);

vec2 GridClipVec2(float granularity, vec2 v);
vec2 GridClipDownVec2(float granularity, vec2 v);

void GridRender(float granularity, float padding, float width, float height, IgnisColorRGBA color);

#endif /* !GRID_H */
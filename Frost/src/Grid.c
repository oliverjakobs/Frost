#include "Grid.h"


float GridClip(float granularity, float f)
{
	return roundf(f / granularity) * granularity;
}

float GridClipDown(float granularity, float f)
{
	return floorf(f / granularity) * granularity;
}

vec2 GridClipVec2(float granularity, vec2 v)
{
	float x = GridClip(granularity, v.x);
	float y = GridClip(granularity, v.y);

	return (vec2) { x, y };
}

vec2 GridClipDownVec2(float granularity, vec2 v)
{
	float x = GridClipDown(granularity, v.x);
	float y = GridClipDown(granularity, v.y);

	return (vec2) { x, y };
}

void GridRender(float granularity, float padding, float width, float height, IgnisColorRGBA color)
{
	for (float x = -padding; x <= width + padding; x += granularity)
		Primitives2DRenderLine(x, -padding, x, height + padding, color);

	for (float y = -padding; y <= height + padding; y += granularity)
		Primitives2DRenderLine(-padding, y, width + padding, y, color);
}

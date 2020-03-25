#ifndef PRIMITIVES2D_H
#define PRIMITIVES2D_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Ignis/Ignis.h"

void Primitives2DInit(const char* vert, const char* frag);
void Primitives2DDestroy();

void Primitives2DStart(const float* mat_view_proj);
void Primitives2DFlush();

void Primitives2DRenderLine(float x1, float y1, float x2, float y2, const IgnisColorRGBA color);
void Primitives2DRenderRect(float x, float y, float w, float h, const IgnisColorRGBA color);
void Primitives2DRenderPolygon(float* vertices, size_t count, const IgnisColorRGBA color);
void Primitives2DRenderCircle(float x, float y, float radius, const IgnisColorRGBA color);


#ifdef __cplusplus
}
#endif

#endif /* !PRIMITIVES2D_H */
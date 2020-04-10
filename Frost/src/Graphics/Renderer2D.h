#ifndef RENDERER2D_H
#define RENDERER2D_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Ignis/Ignis.h"

void Renderer2DInit(const char* vert, const char* frag);
void Renderer2DDestroy();

void Renderer2DRenderTexture(IgnisTexture* texture, float x, float y, float w, float h, const float* mat_view_proj);
void Renderer2DRenderTextureColor(IgnisTexture* texture, float x, float y, float w, float h, IgnisColorRGBA color, const float* mat_view_proj);

#ifdef __cplusplus
}
#endif

#endif /* !RENDERER2D_H */
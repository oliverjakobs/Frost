#ifndef RENDERER2D_H
#define RENDERER2D_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Ignis/Ignis.h"

void Renderer2DInit(const char* vert, const char* frag);
void Renderer2DDestroy();

void Renderer2DStart(const float* mat_view_proj);
void Renderer2DFlush();

void Renderer2DRenderTexture(ignis_texture* texture, const float* mat_model, const float* mat_src);
void Renderer2DRenderTextureColor(ignis_texture* texture, const float* mat_model, const float* mat_src, const ignis_color_rgba color);

#ifdef __cplusplus
}
#endif

#endif /* !RENDERER2D_H */
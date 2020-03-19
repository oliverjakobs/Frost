#ifndef BATCHRENDERER2D_H
#define BATCHRENDERER2D_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Ignis/Ignis.h"

void BatchRenderer2DInit(const char* vert, const char* frag);
void BatchRenderer2DDestroy();

void BatchRenderer2DStart(const float* mat_view_proj);
void BatchRenderer2DFlush();

void BatchRenderer2DRenderTexture(ignis_texture* texture, const float* mat_model, const float* mat_src);
void BatchRenderer2DRenderTextureColor(ignis_texture* texture, const float* mat_model, const float* mat_src, const ignis_color_rgba color);

#ifdef __cplusplus
}
#endif

#endif // !BATCHRENDERER2D_H

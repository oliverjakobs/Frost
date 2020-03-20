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

void BatchRenderer2DRenderTexture(ignis_texture* texture, float x, float y, float width, float height);

#ifdef __cplusplus
}
#endif

#endif /* !BATCHRENDERER2D_H */

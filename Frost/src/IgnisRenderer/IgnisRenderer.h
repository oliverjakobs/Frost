#ifndef IGNISRENDERER_H
#define IGNISRENDERER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "FontRenderer.h"
#include "Primitives2D.h"
#include "BatchRenderer2D.h"
#include "Renderer2D.h"

void GenerateIndices(GLuint* indices, size_t max, size_t step);

#ifdef __cplusplus
}
#endif

#endif /* !IGNISRENDERER_H */

#ifndef FONT_RENDERER_H
#define FONT_RENDERER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Ignis/Ignis.h"

void FontRendererInit(const char* vert, const char* frag);
void FontRendererDestroy();

void FontRendererRenderText(IgnisFont* font, const char* text, float x, float y, const float* mat_proj, const IgnisColorRGBA color);

#ifdef __cplusplus
}
#endif

#endif /* !FONT_RENDERER_H */
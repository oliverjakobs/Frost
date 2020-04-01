#ifndef FONT_RENDERER_H
#define FONT_RENDERER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Ignis/Ignis.h"

void FontRendererInit(const char* vert, const char* frag);
void FontRendererDestroy();

void FontRendererBindFont(IgnisFont* font);
void FontRendererSetColor(IgnisColorRGBA color);

void FontRendererRenderText(float x, float y, const float* mat_proj, const char* text);
void FontRendererRenderTextFormat(float x, float y, const float* mat_proj, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* !FONT_RENDERER_H */
#ifndef IGNIS_FONT_H
#define IGNIS_FONT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Core/Texture.h"

#include "Packages/stb_truetype.h"

typedef struct
{
	int first_char;
	int num_chars;

	int bitmap_width;
	int bitmap_height;

	stbtt_bakedchar* char_data;

	ignis_texture* texture;
} ignis_font;

ignis_font* ignisLoadFont(const char* path, float size);
void ignisDeleteFont(ignis_font* font);

void ignisBindFont(ignis_font* font);
void ignisUnbindFont(ignis_font* font);

int ignisLoadFontCharQuad(ignis_font* font, char c, float* x, float* y, float* vertices);

#ifdef __cplusplus
}
#endif

#endif /* IGNIS_FONT_H */
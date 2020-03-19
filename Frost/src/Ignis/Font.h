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

int ignisLoadFont(ignis_font* font, const char* path, float size);
int ignisLoadFontConfig(ignis_font* font, const char* path, float size, int first, int num, int bm_w, int bm_h);
void ignisDeleteFont(ignis_font* font);

void ignisBindFont(ignis_font* font);
void ignisUnbindFont(ignis_font* font);

int ignisLoadCharQuad(ignis_font* font, char c, float* x, float* y, float* vertices, size_t offset);

#ifdef __cplusplus
}
#endif

#endif /* !IGNIS_FONT_H */
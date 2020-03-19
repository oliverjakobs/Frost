#include "Font.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "Packages/stb_truetype.h"

#include "Ignis.h"

int ignisLoadFont(ignis_font* font, const char* path, float size)
{
	return ignisLoadFontConfig(font, path, size, IGNIS_FONT_FIRST_CHAR, IGNIS_FONT_NUM_CHARS, IGNIS_FONT_BITMAP_WIDTH, IGNIS_FONT_BITMAP_HEIGHT);
}

int ignisLoadFontConfig(ignis_font* font, const char* path, float size, int first, int num, int bm_w, int bm_h)
{
	if (!font)
	{
		_ignisErrorCallback(IGNIS_ERROR, "[Font] Failed to allocate memory");
		return 0;
	}

	font->first_char = first;
	font->num_chars = num;

	font->bitmap_width = bm_w;
	font->bitmap_height = bm_h;

	/* load char data */
	font->char_data = (stbtt_bakedchar*)malloc(sizeof(stbtt_bakedchar) * font->num_chars);
	if (!font->char_data)
	{
		_ignisErrorCallback(IGNIS_ERROR, "[Font] Failed to allocate memory for char data");
	}

	GLubyte* bitmap = (GLubyte*)malloc(sizeof(GLubyte) * font->bitmap_width * font->bitmap_height);
	if (!bitmap)
	{
		_ignisErrorCallback(IGNIS_ERROR, "[Font] Failed to allocate memory for bitmap");
		ignisDeleteFont(font);
		return 0;
	}

	char* buffer = ignisReadFile(path, NULL);
	if (!buffer)
	{
		_ignisErrorCallback(IGNIS_ERROR, "[Font] Failed to read file: %s", path);
		ignisDeleteFont(font);
		free(bitmap);
		return 0;
	}

	stbtt_BakeFontBitmap(buffer, 0, size, bitmap, font->bitmap_width, font->bitmap_height, font->first_char, font->num_chars, font->char_data); // no guarantee this fits!

	free(buffer);

	ignis_texture_config config = IGNIS_DEFAULT_CONFIG;
	config.internal_format = GL_R8;
	config.format = GL_RED;

	font->texture = ignisCreateTextureRawc(font->bitmap_width, font->bitmap_height, bitmap, config);
	if (!font->texture)
	{
		_ignisErrorCallback(IGNIS_ERROR, "[Font] Failed to create texture");
		ignisDeleteFont(font);
		return 0;
	}

	free(bitmap);
	return 1;
}

void ignisDeleteFont(ignis_font* font)
{
	if (font->char_data)
		free(font->char_data);

	if (font->texture)
		ignisDeleteTexture(font->texture);
}

void ignisBindFont(ignis_font* font)
{
	ignisBindTexture(font->texture, 0);
}

void ignisUnbindFont(ignis_font* font)
{
	ignisUnbindTexture(font->texture);
}

int ignisLoadCharQuad(ignis_font* font, char c, float* x, float* y, float* vertices, size_t offset)
{
	if (c >= font->first_char && c < font->first_char + font->num_chars)
	{
		stbtt_aligned_quad q;
		stbtt_GetBakedQuad(font->char_data, font->bitmap_width, font->bitmap_height, c - font->first_char, x, y, &q, 1);

		float quad[] =
		{
			q.x0, q.y0, q.s0, q.t0,
			q.x0, q.y1, q.s0, q.t1,
			q.x1, q.y1, q.s1, q.t1,
			q.x1, q.y0, q.s1, q.t0
		};

		memcpy(vertices + offset, quad, sizeof(quad));

		return 1;
	}

	return 0;
}

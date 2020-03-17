#include "Font.h"

#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "Packages/stb_truetype.h"

#include "Ignis.h"

GLubyte* _ignisReadFontFile(const char* path)
{
	FILE* file = fopen(path, "rb");
	if (!file)
	{
		_ignisErrorCallback(IGNIS_ERROR, "[Font] Failed to open file: %s", path);
		return NULL;
	}

	// find file size
	fseek(file, 0, SEEK_END);
	size_t file_size = ftell(file);
	rewind(file);

	GLubyte* buffer = (GLubyte*)malloc(file_size * sizeof(GLubyte));
	if (!buffer)
	{
		_ignisErrorCallback(IGNIS_ERROR, "[Font] Failed to allocate memory for file: %s", path);
		fclose(file);
		return NULL;
	}

	if (fread(buffer, file_size, 1, file) != 1)
	{
		free(buffer);
		fclose(file);
		return NULL;
	}

	fclose(file);
	return buffer;
}

ignis_font* ignisLoadFont(const char* path, float size)
{
	ignis_font* font = (ignis_font*)malloc(sizeof(ignis_font));

	if (!font)
	{
		_ignisErrorCallback(IGNIS_ERROR, "[Font] Failed to allocate memory");
		return NULL;
	}

	font->first_char = 32;
	font->num_chars = 96; /* ASCII 32..126 is 95 glyphs */

	font->bitmap_width = 512;
	font->bitmap_height = 512;

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
		return NULL;
	}

	GLubyte* buffer = _ignisReadFontFile(path);
	if (!buffer)
	{
		_ignisErrorCallback(IGNIS_ERROR, "[Font] Failed to read file: %s", path);
		ignisDeleteFont(font);
		free(bitmap);
		return NULL;
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
		font = NULL;
	}

	free(bitmap);
	return font;
}

void ignisDeleteFont(ignis_font* font)
{
	if (font->char_data)
		free(font->char_data);

	if (font->texture)
		ignisDeleteTexture(font->texture);

	free(font);
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
	if ((GLchar)c >= font->first_char && (GLchar)c < font->first_char + font->num_chars)
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

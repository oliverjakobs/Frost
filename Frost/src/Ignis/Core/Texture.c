#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../Packages/stb_image.h"

#include "../Ignis.h"

int ignisCreateTextureEmpty(IgnisTexture* texture, int width, int height, IgnisTextureConfig* config)
{
	return ignisCreateTextureRaw(texture, width, height, NULL, config);
}

int ignisCreateTextureRaw(IgnisTexture* texture, int width, int height, void* pixels, IgnisTextureConfig* configptr)
{
	IgnisTextureConfig config = IGNIS_DEFAULT_CONFIG;

	if (configptr)
		config = *configptr;

	if (texture)
	{
		texture->name = ignisGenerateTexture2D(width, height, pixels, config);
		texture->width = width;
		texture->height = height;
		texture->rows = 1;
		texture->columns = 1;
		texture->slot = 0;
		texture->config = config;

		return texture->name;
	}

	return 0;
}

int ignisCreateTexture(IgnisTexture* texture, const char* path, GLuint rows, GLuint columns, int flip_on_load, IgnisTextureConfig* configptr)
{
	IgnisTextureConfig config = IGNIS_DEFAULT_CONFIG;

	if (configptr)
		config = *configptr;

	if (texture)
	{
		stbi_set_flip_vertically_on_load(flip_on_load);

		int bpp = 0;

		unsigned char* pixels = stbi_load(path, &texture->width, &texture->height, &bpp, 4);

		/* check if bpp and format matches */
		if (bpp == 4)
		{
			if (config.format != GL_RGBA || config.internal_format != GL_RGBA8)
				_ignisErrorCallback(IGNIS_WARN, "[Tex] Format mismatch for %s", path);
		}
		else if (bpp == 3)
		{
			if (config.format != GL_RGB || config.internal_format != GL_RGB8)
				_ignisErrorCallback(IGNIS_WARN, "[Tex] Format mismatch for %s", path);
		}

		if (pixels)
		{
			texture->name = ignisGenerateTexture2D(texture->width, texture->height, pixels, config);
			stbi_image_free(pixels);
		}
		else
		{
			_ignisErrorCallback(IGNIS_ERROR, "[Tex] Failed to load texture(%s): %s", path, stbi_failure_reason());
			texture->name = 0;
		}

		texture->rows = rows;
		texture->columns = columns;
		texture->slot = 0;
		texture->config = config;

		return texture->name;
	}

	return 0;
}

void ignisDestroyTexture(IgnisTexture* texture)
{
	glDeleteTextures(1, &texture->name);
}

GLuint ignisGenerateTexture2D(int width, int height, void* pixels, IgnisTextureConfig config)
{
	GLuint name;
	glGenTextures(1, &name);
	glBindTexture(GL_TEXTURE_2D, name);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.wrap_t);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.mag_filter);

	glTexImage2D(GL_TEXTURE_2D, 0, config.internal_format, width, height, 0, config.format, GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	return name;
}

void ignisBindTexture(IgnisTexture* texture, GLuint slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture->name);
	texture->slot = slot;
}

void ignisUnbindTexture(IgnisTexture* texture)
{
	glActiveTexture(GL_TEXTURE0 + texture->slot);
	glBindTexture(GL_TEXTURE_2D, 0);
	texture->slot = 0;
}

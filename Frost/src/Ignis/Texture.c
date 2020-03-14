#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Packages/stb_image.h"

#include "Ignis.h"

ignis_texture* ignisCreateTextureEmpty(int width, int height)
{
	ignis_texture_config config = IGNIS_DEFAULT_CONFIG;
	return ignisCreateTextureRawc(width, height, NULL, config);
}

ignis_texture* ignisCreateTextureEmptyc(int width, int height, ignis_texture_config config)
{
	return ignisCreateTextureRawc(width, height, NULL, config);
}

ignis_texture* ignisCreateTextureRaw(int width, int height, void* pixels)
{
	ignis_texture_config config = IGNIS_DEFAULT_CONFIG;
	return ignisCreateTextureRawc(width, height, pixels, config);
}

ignis_texture* ignisCreateTextureRawc(int width, int height, void* pixels, ignis_texture_config config)
{
	ignis_texture* texture = (ignis_texture*)malloc(sizeof(ignis_texture));

	if (texture)
	{
		texture->name = ignisGenerateTexture2D(width, height, pixels, config);
		texture->width = width;
		texture->height = height;
		texture->rows = 1;
		texture->columns = 1;
		texture->slot = 0;
		texture->config = config;
	}
	else
	{
		_ignisErrorCallback(IGNIS_ERROR, "[Tex] Failed to allocate memory");
	}

	return texture;
}

ignis_texture* ignisCreateTexture(const char* path, GLuint rows, GLuint columns, int flip_on_load)
{
	ignis_texture_config config = IGNIS_DEFAULT_CONFIG;
	return ignisCreateTexturec(path, rows, columns, flip_on_load, config);
}

ignis_texture* ignisCreateTexturec(const char* path, GLuint rows, GLuint columns, int flip_on_load, ignis_texture_config config)
{
	ignis_texture* texture = (ignis_texture*)malloc(sizeof(ignis_texture));

	if (texture)
	{
		stbi_set_flip_vertically_on_load(flip_on_load);

		int bpp = 0;

		unsigned char* pixels = stbi_load(path, &texture->width, &texture->height, &bpp, 4);

		// check if bpp and format matches
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
	}
	else
	{
		_ignisErrorCallback(IGNIS_ERROR, "[Tex] Failed to allocate memory");
	}

	return texture;
}

void ignisDeleteTexture(ignis_texture* texture)
{
	glDeleteTextures(1, &texture->name);
	free(texture);
}

GLuint ignisGenerateTexture2D(int width, int height, void* pixels, ignis_texture_config config)
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

void ignisBindTexture(ignis_texture* texture, GLuint slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture->name);
	texture->slot = slot;
}

void ignisUnbindTexture(ignis_texture* texture)
{
	glActiveTexture(GL_TEXTURE0 + texture->slot);
	glBindTexture(GL_TEXTURE_2D, 0);
	texture->slot = 0;
}

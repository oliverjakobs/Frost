#ifndef IGNIS_TEXTURE_H
#define IGNIS_TEXTURE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Api.h"

	typedef struct
	{
		GLint internal_format;
		GLenum format;

		GLint min_filter;
		GLint mag_filter;

		GLint wrap_s;
		GLint wrap_t;
	} ignis_texture_config;

#define IGNIS_DEFAULT_CONFIG { GL_RGBA8, GL_RGBA, GL_LINEAR, GL_NEAREST, GL_REPEAT, GL_REPEAT }

	typedef struct
	{
		GLuint name;

		int width;
		int height;

		GLuint rows;
		GLuint columns;

		GLuint slot;

		ignis_texture_config config;
	} ignis_texture;

	ignis_texture* ignisCreateTextureEmpty(int width, int height);
	ignis_texture* ignisCreateTextureEmptyc(int width, int height, ignis_texture_config config);
	ignis_texture* ignisCreateTextureRaw(int width, int height, void* pixels);
	ignis_texture* ignisCreateTextureRawc(int width, int height, void* pixels, ignis_texture_config config);
	ignis_texture* ignisCreateTexture(const char* path, GLuint rows, GLuint columns, int flip_on_load);
	ignis_texture* ignisCreateTexturec(const char* path, GLuint rows, GLuint columns, int flip_on_load, ignis_texture_config config);
	void ignisDeleteTexture(ignis_texture* texture);

	GLuint ignisGenerateTexture2D(int width, int height, void* pixels, ignis_texture_config config);

	void ignisBindTexture(ignis_texture* texture, GLuint slot);
	void ignisUnbindTexture(ignis_texture* texture);

#ifdef __cplusplus
}
#endif

#endif /* IGNIS_TEXTURE_H */
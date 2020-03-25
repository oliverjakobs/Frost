#ifndef IGNIS_TEXTURE_H
#define IGNIS_TEXTURE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "../Packages/glad/glad.h"

	typedef struct
	{
		GLint internal_format;
		GLenum format;

		GLint min_filter;
		GLint mag_filter;

		GLint wrap_s;
		GLint wrap_t;
	} IgnisTextureConfig;

#define IGNIS_DEFAULT_CONFIG { GL_RGBA8, GL_RGBA, GL_LINEAR, GL_NEAREST, GL_REPEAT, GL_REPEAT }

	typedef struct
	{
		GLuint name;

		int width;
		int height;

		GLuint rows;
		GLuint columns;

		GLuint slot;

		IgnisTextureConfig config;
	} IgnisTexture;

	int ignisCreateTextureEmpty(IgnisTexture* texture, int width, int height, IgnisTextureConfig* config);
	int ignisCreateTextureRaw(IgnisTexture* texture, int width, int height, void* pixels, IgnisTextureConfig* config);
	int ignisCreateTexture(IgnisTexture* texture, const char* path, GLuint rows, GLuint columns, int flip_on_load, IgnisTextureConfig* config);
	void ignisDestroyTexture(IgnisTexture* texture);

	GLuint ignisGenerateTexture2D(int width, int height, void* pixels, IgnisTextureConfig config);

	void ignisBindTexture(IgnisTexture* texture, GLuint slot);
	void ignisUnbindTexture(IgnisTexture* texture);

#ifdef __cplusplus
}
#endif

#endif /* !IGNIS_TEXTURE_H */
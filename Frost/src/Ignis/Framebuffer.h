#ifndef IGNIS_FRAMEBUFFER_H
#define IGNIS_FRAMEBUFFER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Core/Buffer.h"
#include "Core/Texture.h"

typedef struct
{
	GLuint name;

	IgnisTexture texture;

	int width;
	int height;
} IgnisFrameBuffer;

int ignisGenerateFrameBuffer(IgnisFrameBuffer* fbo, GLuint texture_target, int width, int height);
void ignisDeleteFrameBuffer(IgnisFrameBuffer* fbo);

void ignisBindFrameBuffer(IgnisFrameBuffer* fbo);

#ifdef __cplusplus
}
#endif

#endif /* !IGNIS_FRAMEBUFFER_H */
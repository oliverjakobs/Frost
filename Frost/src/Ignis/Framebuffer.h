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
	IgnisBuffer rbo;

	int width;
	int height;
} IgnisFrameBuffer;

int ignisFrameBufferGenerate(IgnisFrameBuffer* fbo, int width, int height);
void ignisFrameBufferDelete(IgnisFrameBuffer* fbo);

void ignisFrameBufferBind(IgnisFrameBuffer* fbo);
void ignisFrameBufferUnbind();

#ifdef __cplusplus
}
#endif

#endif /* !IGNIS_FRAMEBUFFER_H */
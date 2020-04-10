#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Ignis/Ignis.h"

typedef struct
{
	GLuint name;

	GLuint vao;
	GLuint vbo;

	IgnisTexture texture;
	GLuint rbo;

	int width;
	int height;
} FrameBuffer;

int FrameBufferGenerate(FrameBuffer* fbo, int width, int height);
void FrameBufferDelete(FrameBuffer* fbo);

void FrameBufferBind(FrameBuffer* fbo);
void FrameBufferUnbind();


#endif
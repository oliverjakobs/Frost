#include "Framebuffer.h"

#include "Ignis.h"

int ignisGenerateFrameBuffer(IgnisFrameBuffer* fbo, GLuint texture_target, int width, int height)
{
	fbo->width = width;
	fbo->height = height;

	/* create a color attachment texture */
	ignisCreateTextureEmpty(&fbo->texture, texture_target, width, height, NULL);

	glGenFramebuffers(1, &fbo->name);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->name);

	/* create the actual framebuffer object */
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fbo->texture.name, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		_ignisErrorCallback(IGNIS_CRITICAL, "[Framebuffer] Framebuffer is not complete!");
		ignisDeleteFrameBuffer(fbo);
		return 0;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return 1;
}

void ignisDeleteFrameBuffer(IgnisFrameBuffer* fbo)
{
	ignisDestroyTexture(&fbo->texture);

	glDeleteFramebuffers(1, &fbo->name);
}

void ignisBindFrameBuffer(IgnisFrameBuffer* fbo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, (fbo) ? fbo->name : 0);
}

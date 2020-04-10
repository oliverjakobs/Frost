#include "Framebuffer.h"

#include "Ignis.h"

int ignisFrameBufferGenerate(IgnisFrameBuffer* fbo, int width, int height)
{
	fbo->width = width;
	fbo->height = height;

	/* create a color attachment texture */
	ignisCreateTextureEmpty(&fbo->texture, width, height, NULL);

	/* create a renderbuffer object for depth and stencil attachment */
	ignisGenerateRenderBuffer(&fbo->rbo);
	ignisRenderBufferStorage(&fbo->rbo, GL_DEPTH24_STENCIL8, width, height);

	glGenFramebuffers(1, &fbo->name);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->name);

	/* create the actual framebuffer object */
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo->texture.name, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fbo->rbo.name);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		_ignisErrorCallback(IGNIS_CRITICAL, "Framebuffer is not complete!");
		return 0;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return 1;
}

void ignisFrameBufferDelete(IgnisFrameBuffer* fbo)
{
	ignisDestroyTexture(&fbo->texture);
	ignisDeleteBuffer(&fbo->rbo);

	glDeleteFramebuffers(1, &fbo->name);
}

void ignisFrameBufferBind(IgnisFrameBuffer* fbo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->name);
}

void ignisFrameBufferUnbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

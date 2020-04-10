#include "Framebuffer.h"

int FrameBufferGenerate(FrameBuffer* fbo, int width, int height)
{
	fbo->width = width;
	fbo->height = height;

	float half_w = 1.0f;
	float half_h = 1.0f;

	// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	float quadVertices[] = 
	{ 
		// positions   // texCoords
		-half_w,  half_h,  0.0f, 1.0f,
		-half_w, -half_h,  0.0f, 0.0f,
		 half_w, -half_h,  1.0f, 0.0f,

		-half_w,  half_h,  0.0f, 1.0f,
		 half_w, -half_h,  1.0f, 0.0f,
		 half_w,  half_h,  1.0f, 1.0f
	};

	/* generate vao */
	glGenVertexArrays(1, &fbo->vao);
	glGenBuffers(1, &fbo->vbo);
	glBindVertexArray(fbo->vao);
	glBindBuffer(GL_ARRAY_BUFFER, fbo->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glGenFramebuffers(1, &fbo->name);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->name);
	// create a color attachment texture
	glGenTextures(1, &fbo->texture.name);
	glBindTexture(GL_TEXTURE_2D, fbo->texture.name);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo->texture.name, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &fbo->rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, fbo->rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fbo->rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		_ignisErrorCallback(IGNIS_CRITICAL, "Framebuffer is not complete!");
		return 0;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return 1;
}

void FrameBufferDelete(FrameBuffer* fbo)
{
	glDeleteVertexArrays(1, &fbo->vao);
	glDeleteBuffers(1, &fbo->vao);

	glDeleteTextures(1, &fbo->texture.name);
	glDeleteRenderbuffers(1, &fbo->rbo);

	glDeleteFramebuffers(1, &fbo->name);
}

void FrameBufferBind(FrameBuffer* fbo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->name);
}

void FrameBufferUnbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

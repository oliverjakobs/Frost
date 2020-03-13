#include "Framebuffer.h"

namespace ignis
{
	FrameBuffer::FrameBuffer(int w, int h)
		:m_width(w), m_height(h)
	{
		m_texture = ignisCreateTextureEmpty(w, h);

		if (!m_texture)
			return;

		float vertices[] = {
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		m_vao.Bind();

		m_vbo = ignisGenerateArrayBuffer(sizeof(vertices), vertices, GL_STATIC_DRAW);
		ignisVertexAttribPointer(0, 2, GL_FALSE, 4, 0);
		ignisVertexAttribPointer(1, 2, GL_FALSE, 4, 2);

		glGenFramebuffers(1, &m_name);
		glBindFramebuffer(GL_FRAMEBUFFER, m_name);

		// create a color attachment texture
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->name, 0);

		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		ignis_buffer* rbo = ignisGenerateRenderBuffer();
		ignisRenderBufferStorage(rbo, GL_DEPTH24_STENCIL8, w, h); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo->name); // now actually attach it

		ignisDeleteBuffer(rbo);

		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			_ignisErrorCallback(IGNIS_WARN, "Framebuffer is not complete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		ignisUnbindBuffer(m_vbo);
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &m_name);

		ignisDeleteBuffer(m_vbo);
	}

	void FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_name);
	}

	void FrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::BindVAO()
	{
		m_vao.Bind();
	}

	VertexArray& FrameBuffer::VAO()
	{
		return m_vao;
	}

	ignis_texture* FrameBuffer::Texture()
	{
		return m_texture;
	}
}
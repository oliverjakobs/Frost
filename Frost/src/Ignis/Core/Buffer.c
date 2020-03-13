#include "Buffer.h"

ignis_buffer* _ignisGenerateBuffer(GLenum target, GLenum format)
{
	ignis_buffer* buffer = (ignis_buffer*)malloc(sizeof(ignis_buffer));

	if (!buffer)
	{
		_ignisErrorCallback(IGNIS_ERROR, "[Buffer] Failed to allocate memory");
		return NULL;
	}

	switch (target)
	{
	case GL_TEXTURE_BUFFER:
		glGenTextures(1, &buffer->name);
		break;
	case GL_RENDERBUFFER:
		glGenRenderbuffers(1, &buffer->name);
		break;
	default:
		glGenBuffers(1, &buffer->name);
	}

	buffer->target = target;
	buffer->format = format;
	buffer->count = 0;

	return buffer;
}

ignis_buffer* ignisGenerateArrayBuffer(GLsizeiptr size, const void* data, GLenum usage)
{
	ignis_buffer* buffer = _ignisGenerateBuffer(GL_ARRAY_BUFFER, 0);

	if (buffer)
		ignisBufferData(buffer, size, data, usage);

	return buffer;
}

ignis_buffer* ignisGenerateElementBuffer(GLsizei count, const GLuint* data, GLenum usage)
{
	ignis_buffer* buffer = _ignisGenerateBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (buffer)
		ignisElementBufferData(buffer, count, data, usage);

	return buffer;
}

ignis_buffer* ignisGenerateTextureBuffer(GLenum format, GLuint buffer)
{
	ignis_buffer* tex_buffer = _ignisGenerateBuffer(GL_TEXTURE_BUFFER, format);

	if (tex_buffer)
	{
		glBindTexture(tex_buffer->target, tex_buffer->name);
		glTexBuffer(tex_buffer->target, tex_buffer->format, buffer);
	}

	return tex_buffer;
}

ignis_buffer* ignisGenerateRenderBuffer()
{
	return _ignisGenerateBuffer(GL_RENDERBUFFER, 0);
}

void ignisDeleteBuffer(ignis_buffer* buffer)
{
	switch (buffer->target)
	{
	case GL_TEXTURE_BUFFER:
		glDeleteTextures(1, &buffer->name);
		break;
	case GL_RENDERBUFFER:
		glBindRenderbuffer(buffer->target, 0);
		glDeleteRenderbuffers(1, &buffer->name);
		break;
	default:
		glBindBuffer(buffer->target, 0);
		glDeleteBuffers(1, &buffer->name);
	}

	free(buffer);
}

void ignisBindBuffer(ignis_buffer* buffer)
{
	switch (buffer->target)
	{
	case GL_RENDERBUFFER:
		glBindRenderbuffer(buffer->target, buffer->name);
		break;
	default:
		glBindBuffer(buffer->target, buffer->name);
	}
}

void ignisUnbindBuffer(ignis_buffer* buffer)
{
	switch (buffer->target)
	{
	case GL_RENDERBUFFER:
		glBindRenderbuffer(buffer->target, 0);
		break;
	default:
		glBindBuffer(buffer->target, 0);
	}
}

void ignisBufferData(ignis_buffer* buffer, GLsizeiptr size, const void* data, GLenum usage)
{
	ignisBindBuffer(buffer);
	glBufferData(buffer->target, size, data, usage);
}

void ignisBufferSubData(ignis_buffer* buffer, GLintptr offset, GLsizeiptr size, const void* data)
{
	ignisBindBuffer(buffer);
	glBufferSubData(buffer->target, offset, size, data);
}

void ignisElementBufferData(ignis_buffer* buffer, GLsizei count, const GLuint* data, GLenum usage)
{
	if (buffer->target != GL_ELEMENT_ARRAY_BUFFER)
	{
		_ignisErrorCallback(IGNIS_WARN, "[Buffer] Buffer target is not GL_ELEMENT_ARRAY_BUFFER");
		return;
	}

	ignisBindBuffer(buffer);
	glBufferData(buffer->target, count * sizeof(GLuint), data, usage);
	buffer->count = count;
}

void ignisBindImageTexture(ignis_buffer* buffer, GLuint unit, GLenum access)
{
	if (buffer->target != GL_TEXTURE_BUFFER)
	{
		_ignisErrorCallback(IGNIS_WARN, "[Buffer] Buffer target is not GL_TEXTURE_BUFFER");
		return;
	}

	glBindImageTexture(unit, buffer->name, 0, GL_FALSE, 0, access, buffer->format);
}

void ignisRenderBufferStorage(ignis_buffer* buffer, GLenum format, GLsizei width, GLsizei height)
{
	if (buffer->target != GL_RENDERBUFFER)
	{
		_ignisErrorCallback(IGNIS_WARN, "[Buffer] Buffer target is not GL_RENDERBUFFER");
		return;
	}

	ignisBindBuffer(buffer);
	glRenderbufferStorage(buffer->target, format, width, height);
}

void* ignisMapBuffer(ignis_buffer* buffer, GLenum access)
{
	ignisBindBuffer(buffer);
	return glMapBuffer(buffer->target, access);
}

void* ignisMapBufferRange(ignis_buffer* buffer, GLintptr offset, GLsizeiptr length, GLbitfield access)
{
	ignisBindBuffer(buffer);
	return glMapBufferRange(buffer->target, offset, length, access);
}

void ignisUnmapBuffer(ignis_buffer* buffer)
{
	glUnmapBuffer(buffer->target);
}

void ignisVertexAttribPointerR(GLuint index, GLint size, GLboolean normalized, GLsizei stride, const void* offset)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, normalized ? GL_TRUE : GL_FALSE, stride, offset);
}

void ignisVertexAttribPointer(GLuint index, GLint size, GLboolean normalized, GLsizei stride, GLintptr offset)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, normalized ? GL_TRUE : GL_FALSE, stride * sizeof(GLfloat), (void*)(offset * sizeof(GLfloat)));
}

void ignisVertexAttribIPointer(GLuint index, GLint size, GLsizei stride, GLintptr offset)
{
	glEnableVertexAttribArray(index);
	glVertexAttribIPointer(index, size, GL_UNSIGNED_INT, stride * sizeof(GLuint), (void*)(offset * sizeof(GLuint)));
}

void ignisVertexAttribDivisor(GLuint index, GLuint divisor)
{
	glVertexAttribDivisor(index, divisor);
}



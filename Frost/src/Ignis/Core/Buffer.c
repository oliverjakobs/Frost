#include "Buffer.h"

void ignisBindBuffer(GLenum target, GLuint name)
{
	if (target == GL_RENDERBUFFER)
		glBindRenderbuffer(target, name);
	else
		glBindBuffer(target, name);
}

void ignisUnbindBuffer(GLenum target)
{
	if (target == GL_RENDERBUFFER)
		glBindRenderbuffer(target, 0);
	else
		glBindBuffer(target, 0);
}

ignis_array_buffer* ignisGenerateArrayBuffer(GLsizeiptr size, const void* data, GLenum usage)
{
	ignis_array_buffer* buffer = (ignis_array_buffer*)malloc(sizeof(ignis_array_buffer));

	if (buffer == NULL)
	{
		_ignisErrorCallback(IGNIS_ERROR, "[Buffer] Failed to allocate memory");
		return NULL;
	}

	glGenBuffers(1, &buffer->name);
	buffer->target = GL_ARRAY_BUFFER;

	ignisArrayBufferData(buffer, size, data, usage);

	return buffer;
}

void ignisDeleteArrayBuffer(ignis_array_buffer* buffer)
{
	glDeleteBuffers(1, &buffer->name);
	glBindBuffer(buffer->target, 0);

	free(buffer);
}

void ignisArrayBufferData(ignis_array_buffer* buffer, GLsizeiptr size, const void* data, GLenum usage)
{
	ignisBindBuffer(buffer->target, buffer->name);
	glBufferData(buffer->target, size, data, usage);
}

void ignisArrayBufferSubData(ignis_array_buffer* buffer, GLintptr offset, GLsizeiptr size, const void* data)
{
	ignisBindBuffer(buffer->target, buffer->name);
	glBufferSubData(buffer->target, offset, size, data);
}

void* ignisMapArrayBuffer(ignis_array_buffer* buffer, GLenum access)
{
	ignisBindBuffer(buffer->target, buffer->name);
	return glMapBuffer(buffer->target, access);
}

void* ignisMapArrayBufferRange(ignis_array_buffer* buffer, GLintptr offset, GLsizeiptr length, GLbitfield access)
{
	ignisBindBuffer(buffer->target, buffer->name);
	return glMapBufferRange(buffer->target, offset, length, access);
}

void ignisUnmapArrayBuffer(ignis_array_buffer* buffer)
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

ignis_element_buffer* ignisGenerateElementBuffer(GLsizei count, const GLuint* data, GLenum usage)
{
	ignis_element_buffer* buffer = (ignis_element_buffer*)malloc(sizeof(ignis_element_buffer));

	if (buffer == NULL)
	{
		_ignisErrorCallback(IGNIS_ERROR, "[Buffer] Failed to allocate memory");
		return NULL;
	}

	glGenBuffers(1, &buffer->name);
	buffer->target = GL_ELEMENT_ARRAY_BUFFER;

	ignisElementBufferData(buffer, count, data, usage);

	return buffer;
}

void ignisDeleteElementBuffer(ignis_element_buffer* buffer)
{
	glDeleteBuffers(1, &buffer->name);
	glBindBuffer(buffer->target, 0);

	free(buffer);
}

void ignisElementBufferData(ignis_element_buffer* buffer, GLsizei count, const GLuint* data, GLenum usage)
{
	ignisBindBuffer(buffer->target, buffer->name);
	glBufferData(buffer->target, count * sizeof(GLuint), data, usage);
	buffer->count = count;
}

ignis_texture_buffer* ignisGenerateTextureBuffer(GLenum format, GLuint buffer)
{
	ignis_texture_buffer* tex_buffer = (ignis_texture_buffer*)malloc(sizeof(ignis_texture_buffer));

	if (tex_buffer == NULL)
	{
		_ignisErrorCallback(IGNIS_ERROR, "[Buffer] Failed to allocate memory");
		return NULL;
	}

	glGenTextures(1, &tex_buffer->texture);
	tex_buffer->format = format;
	tex_buffer->target = GL_TEXTURE_BUFFER;

	glBindTexture(tex_buffer->target, tex_buffer->texture);
	glTexBuffer(tex_buffer->target, tex_buffer->format, buffer);

	return tex_buffer;
}

void ignisDeleteTextureBuffer(ignis_texture_buffer* buffer)
{
	glDeleteTextures(1, &buffer->texture);

	free(buffer);
}

void ignisBindImageTexture(ignis_texture_buffer* buffer, GLuint unit, GLenum access)
{
	glBindImageTexture(unit, buffer->texture, 0, GL_FALSE, 0, access, buffer->format);
}

ignis_render_buffer* ignisGenerateRenderBuffer()
{
	ignis_render_buffer* buffer = (ignis_render_buffer*)malloc(sizeof(ignis_render_buffer));

	if (buffer == NULL)
	{
		_ignisErrorCallback(IGNIS_ERROR, "[Buffer] Failed to allocate memory");
		return NULL;
	}

	glGenRenderbuffers(1, &buffer->name);
	buffer->target = GL_RENDERBUFFER;

	return buffer;
}

void ignisDeleteRenderBuffer(ignis_render_buffer* buffer)
{
	glDeleteRenderbuffers(1, &buffer->name);

	free(buffer);
}

void ignisRenderBufferStorage(ignis_render_buffer* buffer, GLenum format, GLsizei width, GLsizei height)
{
	ignisBindBuffer(buffer->target, buffer->name);
	glRenderbufferStorage(buffer->target, format, width, height);
}
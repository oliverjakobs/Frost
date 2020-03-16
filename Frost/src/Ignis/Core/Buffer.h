#ifndef IGNIS_BUFFER_H
#define IGNIS_BUFFER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "../Packages/glad/glad.h"

	/* Buffer */
	typedef struct
	{
		GLuint name;
		GLenum target;
		GLenum format;
		GLsizei count;
	} ignis_buffer;

	/* buffer generation */
	ignis_buffer* ignisGenerateArrayBuffer(GLsizeiptr size, const void* data, GLenum usage);
	ignis_buffer* ignisGenerateElementBuffer(GLsizei count, const GLuint* data, GLenum usage);
	ignis_buffer* ignisGenerateTextureBuffer(GLenum format, GLuint buffer);
	ignis_buffer* ignisGenerateRenderBuffer();

	void ignisDeleteBuffer(ignis_buffer* buffer);

	/* buffer binding */
	void ignisBindBuffer(ignis_buffer* buffer);
	void ignisUnbindBuffer(ignis_buffer* buffer);

	/* buffer data */
	void ignisBufferData(ignis_buffer* buffer, GLsizeiptr size, const void* data, GLenum usage);
	void ignisBufferSubData(ignis_buffer* buffer, GLintptr offset, GLsizeiptr size, const void* data);

	void ignisElementBufferData(ignis_buffer* buffer, GLsizei count, const GLuint* data, GLenum usage);

	void ignisBindImageTexture(ignis_buffer* buffer, GLuint unit, GLenum access);
	void ignisRenderBufferStorage(ignis_buffer* buffer, GLenum format, GLsizei width, GLsizei height);

	/* buffer mapping */
	void* ignisMapBuffer(ignis_buffer* buffer, GLenum access);
	void* ignisMapBufferRange(ignis_buffer* buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
	void ignisUnmapBuffer(ignis_buffer* buffer);

	/* vertex attrib pointer */
	void ignisVertexAttribPointerR(GLuint index, GLint size, GLboolean normalized, GLsizei stride, const void* offset);
	void ignisVertexAttribPointer(GLuint index, GLint size, GLboolean normalized, GLsizei stride, GLintptr offset);
	void ignisVertexAttribIPointer(GLuint index, GLint size, GLsizei stride, GLintptr offset);
	void ignisVertexAttribDivisor(GLuint index, GLuint divisor);

#ifdef __cplusplus
}
#endif

#endif /* IGNIS_BUFFER_H */
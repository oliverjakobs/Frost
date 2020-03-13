#ifndef IGNIS_BUFFER_H
#define IGNIS_BUFFER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Api.h"

	/* Buffer */
	void ignisBindBuffer(GLenum target, GLuint name);
	void ignisUnbindBuffer(GLenum target);

	/* ArrayBuffer */
	typedef struct
	{
		GLuint name;
		GLenum target;
	} ignis_array_buffer;

	ignis_array_buffer* ignisGenerateArrayBuffer(GLsizeiptr size, const void* data, GLenum usage);
	void ignisDeleteArrayBuffer(ignis_array_buffer* buffer);

	void ignisArrayBufferData(ignis_array_buffer* buffer, GLsizeiptr size, const void* data, GLenum usage);
	void ignisArrayBufferSubData(ignis_array_buffer* buffer, GLintptr offset, GLsizeiptr size, const void* data);

	void* ignisMapArrayBuffer(ignis_array_buffer* buffer, GLenum access);
	void* ignisMapArrayBufferRange(ignis_array_buffer* buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
	void ignisUnmapArrayBuffer(ignis_array_buffer* buffer);

	void ignisVertexAttribPointerR(GLuint index, GLint size, GLboolean normalized, GLsizei stride, const void* offset);
	void ignisVertexAttribPointer(GLuint index, GLint size, GLboolean normalized, GLsizei stride, GLintptr offset);
	void ignisVertexAttribIPointer(GLuint index, GLint size, GLsizei stride, GLintptr offset);
	void ignisVertexAttribDivisor(GLuint index, GLuint divisor);

	/* ElementBuffer */
	typedef struct
	{
		GLuint name;
		GLenum target;
		GLsizei count;
	} ignis_element_buffer;

	ignis_element_buffer* ignisGenerateElementBuffer(GLsizei count, const GLuint* data, GLenum usage);
	void ignisDeleteElementBuffer(ignis_element_buffer* buffer);

	void ignisElementBufferData(ignis_element_buffer* buffer, GLsizei count, const GLuint* data, GLenum usage);

	/* TextureBuffer */
	typedef struct
	{
		GLuint texture;
		GLenum target;
		GLenum format;
	} ignis_texture_buffer;

	ignis_texture_buffer* ignisGenerateTextureBuffer(GLenum format, GLuint buffer);
	void ignisDeleteTextureBuffer(ignis_texture_buffer* buffer);

	void ignisBindImageTexture(ignis_texture_buffer* buffer, GLuint unit, GLenum access);

	/* RenderBuffer */
	typedef struct
	{
		GLuint name;
		GLenum target;
	} ignis_render_buffer;

	ignis_render_buffer* ignisGenerateRenderBuffer();
	void ignisDeleteRenderBuffer(ignis_render_buffer* buffer);

	void ignisRenderBufferStorage(ignis_render_buffer* buffer, GLenum format, GLsizei width, GLsizei height);

#ifdef __cplusplus
}
#endif

#endif /* IGNIS_BUFFER_H */
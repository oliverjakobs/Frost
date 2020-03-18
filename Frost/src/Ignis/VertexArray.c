#include "VertexArray.h"

#include "Ignis.h"

void ignisGenerateVertexArray(ignis_vertex_array* vao)
{
	if (vao)
	{
		glGenVertexArrays(1, &vao->name);
		glBindVertexArray(vao->name);

		vao->vertex_attrib_index = 0;

		vao->array_buffers = (ignis_buffer*)malloc(IGNIS_BUFFER_ARRAY_INITIAL_SIZE * sizeof(ignis_buffer));
		vao->buffer_count = IGNIS_BUFFER_ARRAY_INITIAL_SIZE;
		vao->buffer_used = 0;

		vao->element_buffer.name = 0;
		vao->element_buffer.target = 0;
		vao->element_buffer.format = 0;
		vao->element_buffer.count = 0;
	}
}

void ignisDeleteVertexArray(ignis_vertex_array* vao)
{
	for (size_t i = 0; i < vao->buffer_used; i++)
	{
		ignisDeleteBuffer(&vao->array_buffers[i]);
	}

	free(vao->array_buffers);
	vao->buffer_used = vao->buffer_count = 0;

	if (vao->element_buffer.target == GL_ELEMENT_ARRAY_BUFFER)
		ignisDeleteBuffer(&vao->element_buffer);

	glDeleteVertexArrays(1, &vao->name);
	glBindVertexArray(0);
}

void ignisBindVertexArray(ignis_vertex_array* vao)
{
	glBindVertexArray(vao->name);
}

void ignisUnbindVertexArray(ignis_vertex_array* vao)
{
	glBindVertexArray(0);
}

void _ignisInsertArrayBuffer(ignis_vertex_array* vao, ignis_buffer* buffer)
{
	if (vao->buffer_used == vao->buffer_count)
	{
		vao->buffer_count *= IGNIS_BUFFER_ARRAY_GROWTH_FACTOR;
		void* temp = realloc(vao->array_buffers, vao->buffer_count * sizeof(ignis_buffer));

		if (!temp)
		{
			_ignisErrorCallback(IGNIS_ERROR, "[VAO] Failed to grow dynamic buffer array");
			return;
		}

		vao->array_buffers = (ignis_buffer*)temp;
	}

	memcpy(vao->array_buffers + vao->buffer_used++, buffer, sizeof(ignis_buffer));
}

void ignisAddArrayBuffer(ignis_vertex_array* vao, GLsizeiptr size, const void* data, GLenum usage)
{
	ignisBindVertexArray(vao);

	ignis_buffer buffer;
	ignisGenerateArrayBuffer(&buffer, size, data, usage);

	_ignisInsertArrayBuffer(vao, &buffer);
}

void ignisAddArrayBufferLayout(ignis_vertex_array* vao, GLsizeiptr size, const void* data, GLenum usage, ignis_buffer_element* layout, size_t count)
{
	ignisBindVertexArray(vao);

	ignis_buffer buffer;
	ignisGenerateArrayBuffer(&buffer, size, data, usage);

	_ignisInsertArrayBuffer(vao, &buffer);

	unsigned int stride = 0;
	for (size_t i = 0; i < count; i++)
	{
		stride += ignisGetOpenGLTypeSize(layout[i].type) * layout[i].count;
	}

	unsigned offset = 0;
	for (size_t i = 0; i < count; i++)
	{
		glEnableVertexAttribArray(vao->vertex_attrib_index);
		glVertexAttribPointer(vao->vertex_attrib_index, layout[i].count, layout[i].type, layout[i].normalized ? GL_TRUE : GL_FALSE, stride, (const void*)(intptr_t)offset);

		offset += ignisGetOpenGLTypeSize(layout[i].type) * layout[i].count;
		vao->vertex_attrib_index++;
	}
}

void ignisLoadElementBuffer(ignis_vertex_array* vertex_array, GLuint* indices, GLsizei count, GLenum usage)
{
	 ignisGenerateElementBuffer(&vertex_array->element_buffer, count, indices, usage);
}
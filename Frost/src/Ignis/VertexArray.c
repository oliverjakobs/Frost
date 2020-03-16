#include "VertexArray.h"

#include "Ignis.h"

#define INITIAL_SIZE 4

ignis_vertex_array* ignisGenerateVertexArray()
{
	ignis_vertex_array* vertex_array = (ignis_vertex_array*)malloc(sizeof(ignis_vertex_array));

	if (vertex_array)
	{
		glGenVertexArrays(1, &vertex_array->name);
		glBindVertexArray(vertex_array->name);

		vertex_array->vertex_attrib_index = 0;

		vertex_array->array_buffers = (ignis_buffer**)malloc(INITIAL_SIZE * sizeof(ignis_buffer));
		vertex_array->array_size = INITIAL_SIZE;
		vertex_array->buffer_count = 0;

		return vertex_array;
	}

	return NULL;
}

void ignisDeleteVertexArray(ignis_vertex_array* vertex_array)
{
	for (size_t i = 0; i < vertex_array->buffer_count; i++)
	{
		ignisDeleteBuffer(vertex_array->array_buffers[i]);
	}

	free(vertex_array->array_buffers);
	vertex_array->array_buffers = NULL;
	vertex_array->buffer_count = vertex_array->array_size = 0;

	if (vertex_array->element_buffer)
		ignisDeleteBuffer(vertex_array->element_buffer);


	glDeleteVertexArrays(1, &vertex_array->name);
	glBindVertexArray(0);

	free(vertex_array);
}

void ignisBindVertexArray(ignis_vertex_array* vertex_array)
{
	glBindVertexArray(vertex_array->name);
}

void ignisUnbindVertexArray(ignis_vertex_array* vertex_array)
{
	glBindVertexArray(0);
}

void _ignisInsertArrayBuffer(ignis_vertex_array* vertex_array, ignis_buffer* buffer)
{
	if (vertex_array->buffer_count == vertex_array->array_size) 
	{
		vertex_array->array_size *= 2;
		vertex_array->array_size = (ignis_buffer**)realloc(vertex_array->array_buffers, vertex_array->array_size * sizeof(ignis_buffer));
	}

	vertex_array->array_buffers[vertex_array->buffer_count++] = buffer;
}

void ignisAddArrayBuffer(ignis_vertex_array* vertex_array, ignis_buffer* buffer)
{
	if (buffer->target != GL_ARRAY_BUFFER)
	{
		_ignisErrorCallback(IGNIS_WARN, "[VertexArray] Buffer target is not GL_ARRAY_BUFFER");
		return;
	}

	ignisBindVertexArray(vertex_array);
	ignisBindBuffer(buffer);

	_ignisInsertArrayBuffer(vertex_array, buffer);
}

void ignisAddArrayBufferLayout(ignis_vertex_array* vertex_array, GLsizeiptr size, const void* data, GLenum usage, ignis_buffer_element* elements, size_t count)
{
	ignisBindVertexArray(vertex_array);

	ignis_buffer* buffer = ignisGenerateArrayBuffer(size, data, usage);
	ignisBindBuffer(buffer);

	unsigned int stride = 0;
	for (size_t i = 0; i < count; i++)
	{
		stride += ignisGetOpenGLTypeSize(elements[i].type) * elements[i].count;
	}

	unsigned offset = 0;
	for (size_t i = 0; i < count; i++)
	{
		glEnableVertexAttribArray(vertex_array->vertex_attrib_index);
		glVertexAttribPointer(vertex_array->vertex_attrib_index, elements[i].count, elements[i].type, elements[i].normalized ? GL_TRUE : GL_FALSE, stride, (const void*)(intptr_t)offset);

		offset += ignisGetOpenGLTypeSize(elements[i].type) * elements[i].count;
		vertex_array->vertex_attrib_index++;
	}

	_ignisInsertArrayBuffer(vertex_array, buffer);
}

void ignisLoadElementBuffer(ignis_vertex_array* vertex_array, GLuint* indices, size_t count, GLenum usage)
{
	vertex_array->element_buffer = ignisGenerateElementBuffer(count, indices, usage);
}
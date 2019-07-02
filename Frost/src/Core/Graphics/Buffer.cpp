#include "Buffer.h"

VAO::VAO()
{
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &id);
	glDeleteBuffers(buffers.size(), buffers.data());
}

void VAO::Bind() const
{
	glBindVertexArray(id);
}

void VAO::Unbind() const
{
	glBindVertexArray(0);
}

unsigned int VAO::BindVertexBuffer()
{
	unsigned int vbo;

	glGenBuffers(1, &vbo);

	buffers.push_back(vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	return vbo;
}

void VAO::UnbindVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VAO::SetVertexBufferData(size_t size, const void* data, int usage)
{
	glBufferData(GL_ARRAY_BUFFER, size, data, (GLenum)usage);
}

void VAO::SetVertexAttribPointer(unsigned int index, int size, size_t stride, int offset)
{
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
	glEnableVertexAttribArray(index);
}
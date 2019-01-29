#pragma once

#include <glad\glad.h>

struct VBO
{
	unsigned int id;

	VBO()
	{
		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}

	~VBO()
	{
		glDeleteBuffers(1, &id);
	}

	void bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}

	void setBufferData(size_t size, const void* data, int usage = GL_STATIC_DRAW)
	{
		glBufferData(GL_ARRAY_BUFFER, size, data, (GLenum)usage);
	}

	void setAttribPointer(unsigned int index, int size, size_t stride, int offset)
	{
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(index);
	}
};
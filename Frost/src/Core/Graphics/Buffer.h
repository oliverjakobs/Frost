#pragma once

#include <glad/glad.h>
#include <vector>

struct VAO
{
	unsigned int id;

	std::vector<unsigned int> buffers;

	VAO();
	~VAO();

	void Bind() const;
	void Unbind() const;

	// vertex buffers
	unsigned int BindVertexBuffer();
	void UnbindVertexBuffer();

	void SetVertexBufferData(size_t size, const void* data, int usage = GL_STATIC_DRAW);
	void SetVertexAttribPointer(unsigned int index, int size, size_t stride, int offset);
};
#pragma once

#include <glad/glad.h>

namespace sb
{
	struct VAO
	{
		unsigned int id;

		std::vector<unsigned int> buffers;

		VAO()
		{
			glGenVertexArrays(1, &id);
			glBindVertexArray(id);
		}

		~VAO()
		{
			glDeleteVertexArrays(1, &id);

			glDeleteBuffers(buffers.size(), buffers.data());
		}

		void bind() const
		{
			glBindVertexArray(id);
		}

		void unbind() const
		{
			glBindVertexArray(0);
		}

		unsigned int bindVertexBuffer()
		{
			unsigned int vbo;

			glGenBuffers(1, &vbo);

			buffers.push_back(vbo);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);

			return vbo;
		}

		void setVertexBufferData(size_t size, const void* data, int usage = GL_STATIC_DRAW)
		{
			glBufferData(GL_ARRAY_BUFFER, size, data, (GLenum)usage);
		}

		void setVertexAttribPointer(unsigned int index, int size, size_t stride, int offset)
		{
			glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
			glEnableVertexAttribArray(index);
		}
	};
}
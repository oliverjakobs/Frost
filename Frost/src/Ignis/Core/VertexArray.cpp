#include "VertexArray.hpp"

namespace ignis
{
	// VertexArray
	VertexArray::VertexArray() : m_vertexAttribIndex(0), m_elementBuffer(NULL)
	{
		glGenVertexArrays(1, &m_name);
		glBindVertexArray(m_name);
	}

	VertexArray::~VertexArray()
	{
		for (auto& buffer : m_arrayBuffers)
		{
			ignisDeleteArrayBuffer(buffer);
		}

		if (m_elementBuffer)
			ignisDeleteElementBuffer(m_elementBuffer);

		glDeleteVertexArrays(1, &m_name);
		glBindVertexArray(0);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_name);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddArrayBuffer(ignis_array_buffer* buffer)
	{
		Bind();
		ignisBindBuffer(buffer->target, buffer->name);

		m_arrayBuffers.push_back(buffer);
	}

	void VertexArray::AddArrayBuffer(GLsizeiptr size, const void* data, GLenum usage)
	{
		AddArrayBuffer(ignisGenerateArrayBuffer(size, data, usage));
	}

	void VertexArray::AddArrayBuffer(GLsizeiptr size, const void* data, GLenum usage, const BufferLayout& layout)
	{
		Bind();

		auto buffer = ignisGenerateArrayBuffer(size, data, usage);
		ignisBindBuffer(buffer->target, buffer->name);

		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(m_vertexAttribIndex);
			glVertexAttribPointer(m_vertexAttribIndex, element.Count, element.Type, element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)(intptr_t)element.Offset);

			m_vertexAttribIndex++;
		}

		m_arrayBuffers.push_back(buffer);
	}

	void VertexArray::LoadElementBuffer(std::vector<GLuint> indices, GLenum usage)
	{
		m_elementBuffer = ignisGenerateElementBuffer(indices.size(), indices.data(), usage);
	}
}
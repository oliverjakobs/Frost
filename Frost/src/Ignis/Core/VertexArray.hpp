#pragma once

#include "Buffer.h"

#include <vector>
#include <memory>

namespace ignis
{
	// VertexArray
	static unsigned int GetOpenGLTypeSize(GLenum type)
	{
		switch (type)
		{
		case GL_FLOAT:			return sizeof(GLfloat);
		case GL_INT:			return sizeof(GLint);
		case GL_UNSIGNED_INT:	return sizeof(GLuint);
		default: return 0;
		}
	}

	struct BufferElement
	{
		GLenum Type;
		GLsizei Count;
		GLuint Offset;
		GLboolean Normalized;

		BufferElement() : Type(GL_UNSIGNED_INT), Count(0), Offset(0), Normalized(GL_FALSE) { }
		BufferElement(GLenum type, GLsizei count, GLboolean normalized = GL_FALSE) : Type(type), Count(count), Offset(0), Normalized(normalized) { }
	};

	class BufferLayout
	{
	private:
		std::vector<BufferElement> m_elements;
		GLuint m_stride = 0;

	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_elements(elements)
		{
			// Calculate offsets and stride
			GLuint offset = 0;
			m_stride = 0;
			for (auto& element : m_elements)
			{
				element.Offset = offset;
				GLuint size = GetOpenGLTypeSize(element.Type) * element.Count;

				offset += size;
				m_stride += size;
			}
		}

		inline GLuint GetStride() const { return m_stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_elements; }

		std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }
	};

	class VertexArray
	{
	private:
		GLuint m_name;

		GLuint m_vertexAttribIndex;

		std::vector<ignis_buffer*> m_arrayBuffers;
		ignis_buffer* m_elementBuffer;

	public:
		VertexArray();
		~VertexArray();

		void Bind() const;
		void Unbind() const;

		void AddArrayBuffer(ignis_buffer* buffer);
		void AddArrayBuffer(GLsizeiptr size, const void* data, GLenum usage);
		void AddArrayBuffer(GLsizeiptr size, const void* data, GLenum usage, const BufferLayout& layout);
		void LoadElementBuffer(std::vector<GLuint> indices, GLenum usage);

		const GLuint GetName() const { return m_name; }
		const GLsizei GetElementCount() const { return (m_elementBuffer ? m_elementBuffer->count : 0); }

		const std::vector<ignis_buffer*>& GetArrayBuffers() const { return m_arrayBuffers; }
		const ignis_buffer* GetElementBuffer() const { return m_elementBuffer; }

		ignis_buffer* GetArrayBuffer(GLsizei index) const { return m_arrayBuffers.at(index); }
	};
}
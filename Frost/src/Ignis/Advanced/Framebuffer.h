#pragma once

#include "Ignis/Texture.h"
#include "Ignis/VertexArray.hpp"

namespace ignis
{
	class FrameBuffer
	{
	private:
		GLuint m_name;
		ignis_texture* m_texture;

		VertexArray m_vao;
		ignis_buffer* m_vbo;

		int m_width, m_height;
	public:
		FrameBuffer(int w, int h);
		~FrameBuffer();

		void Bind();
		void Unbind();

		void BindVAO();

		VertexArray& VAO();
		ignis_texture* Texture();
	};
}
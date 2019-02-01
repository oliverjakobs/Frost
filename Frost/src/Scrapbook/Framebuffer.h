#pragma once

namespace sb
{
	class FrameBuffer
	{
	private:
		unsigned int m_fbo;
		unsigned int m_vao;

		int m_width;
		int m_height;

		unsigned int m_texture;
	public:
		FrameBuffer(int w, int h);
		~FrameBuffer();

		void bind();
		void unbind();

		void render();
	};
}
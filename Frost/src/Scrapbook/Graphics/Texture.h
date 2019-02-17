#pragma once

#include <glad\glad.h>

namespace sb
{
	class Texture
	{
	private:
		unsigned int m_id;
		int m_width;
		int m_height;
		int m_bpp;

		unsigned int m_slot;
	public:
		Texture(const char* path, unsigned int slot = 0, int minFilter = GL_NEAREST, int magFilter = GL_NEAREST);
		Texture(float width, float height, unsigned int slot = 0, int minFilter = GL_NEAREST, int magFilter = GL_NEAREST);
		~Texture();

		static unsigned int CreateTexture(GLubyte* pixels, float width, float height, int minFilter, int magFilter);

		void bind();

		inline unsigned int getID() const { return m_id; }
		inline unsigned int getSlot() const { return m_slot; }
	};
}
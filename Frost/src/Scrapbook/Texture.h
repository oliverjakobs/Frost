#pragma once

namespace sb
{
	class Texture
	{
	private:
		unsigned int id;
		int width;
		int height;
		int bpp;

		unsigned int m_slot;
	public:
		Texture(const char* path, unsigned int slot);
		~Texture();

		void bind();

		inline unsigned int getSlot() const { return m_slot; }
	};
}
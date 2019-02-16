#pragma once

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
		Texture(const char* path, unsigned int slot);
		~Texture();

		void bind();

		inline unsigned int getSlot() const { return m_slot; }
	};
}
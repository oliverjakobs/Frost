#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Scrapbook/Utility.h"

namespace sb
{
	Texture::Texture(const char* path, unsigned int slot, int minFilter, int magFilter) : m_slot(slot)
	{
		stbi_set_flip_vertically_on_load(true);

		m_width = 0;
		m_height = 0;
		m_bpp = 0;

		GLubyte* pixels = stbi_load(path, &m_width, &m_height, &m_bpp, 4);

		if (pixels)
		{
			m_id = CreateTexture(pixels, m_width, m_height, minFilter, magFilter);
			stbi_image_free(pixels);
		}
		else
		{
			DEBUG_MESSAGE("Failed to load Texture: " << path);
		}
	}

	Texture::Texture(float width, float height, unsigned int slot, int minFilter, int magFilter)
		: m_width(width), m_height(height)
	{
		m_id = CreateTexture(nullptr, m_width, m_height, minFilter, magFilter);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_id);
		m_id = 0;
	}

	unsigned int Texture::CreateTexture(GLubyte* pixels, float width, float height, int minFilter, int magFilter)
	{
		unsigned int id;
		glGenTextures(1, &id);

		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		return id;
	}

	void Texture::bind()
	{
		glActiveTexture(GL_TEXTURE0 + m_slot);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}
}
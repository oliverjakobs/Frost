#include "Texture.h"

#include <glad\glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Scrapbook/Utility.h"

namespace sb
{
	Texture::Texture(const char * path, unsigned int slot) : m_slot(slot)
	{
		glGenTextures(1, &m_id);
		stbi_set_flip_vertically_on_load(true);

		m_width = 0;
		m_height = 0;
		m_bpp = 0;

		GLubyte* pixels = stbi_load(path, &m_width, &m_height, &m_bpp, 4);

		if (pixels)
		{
			glBindTexture(GL_TEXTURE_2D, m_id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			stbi_image_free(pixels);
		}
		else
		{
			DEBUG_MESSAGE("Failed to load Texture: " << path);
		}
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_id);
		m_id = 0;
	}

	void Texture::bind()
	{
		glActiveTexture(GL_TEXTURE0 + m_slot);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}
}
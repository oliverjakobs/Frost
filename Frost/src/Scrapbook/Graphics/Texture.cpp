#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Scrapbook/Utility.h"

namespace sb
{
	Texture::Texture(const char* path, unsigned int slot)
		: m_slot(slot)
	{
		stbi_set_flip_vertically_on_load(true);

		m_width = 0;
		m_height = 0;
		m_bpp = 0;

		unsigned char* pixels = stbi_load(path, &m_width, &m_height, &m_bpp, 4);

		if (pixels)
		{
			TextureConfig config;

			config.INTERAL_FORMAT = GL_RGBA8;
			config.FORMAT = GL_RGBA;
			config.TYPE = GL_UNSIGNED_BYTE;
			config.MIN_FILTER = GL_LINEAR;
			config.MAG_FILTER = GL_NEAREST;
			config.WRAP_S = GL_REPEAT;
			config.WRAP_T = GL_REPEAT;

			m_id = CreateTexture(pixels, m_width, m_height, config);
			stbi_image_free(pixels);
		}
		else
		{
			DEBUG_MESSAGE("Failed to load Texture: " << path);
			m_id = 0;
		}
	}

	Texture::Texture(int width, int height, unsigned int slot)
		: m_width(width), m_height(height), m_slot(slot)
	{
		TextureConfig config;

		config.INTERAL_FORMAT = GL_RGBA8;
		config.FORMAT = GL_RGBA;
		config.TYPE = GL_UNSIGNED_BYTE;
		config.MIN_FILTER = GL_LINEAR;
		config.MAG_FILTER = GL_NEAREST;
		config.WRAP_S = GL_REPEAT;
		config.WRAP_T = GL_REPEAT;

		m_id = CreateTexture(nullptr, m_width, m_height, config);
	}

	Texture::Texture(const char* path, TextureConfig config, unsigned int slot)
		: m_slot(slot)
	{
		stbi_set_flip_vertically_on_load(true);

		m_width = 0;
		m_height = 0;
		m_bpp = 0;

		unsigned char* pixels = stbi_load(path, &m_width, &m_height, &m_bpp, 4);

		if (pixels)
		{
			m_id = CreateTexture(pixels, m_width, m_height, config);
			stbi_image_free(pixels);
		}
		else
		{
			DEBUG_MESSAGE("Failed to load Texture: " << path);
			m_id = 0;
		}
	}

	Texture::Texture(int width, int height, TextureConfig config, unsigned int slot)
		: m_width(width), m_height(height), m_slot(slot)
	{
		m_id = CreateTexture(nullptr, m_width, m_height, config);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_id);
		m_id = 0;
	}

	unsigned int Texture::CreateTexture(unsigned char* pixels, int width, int height, TextureConfig config)
	{
		unsigned int id;
		glGenTextures(1, &id);

		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, config.INTERAL_FORMAT, width, height, 0, config.FORMAT, config.TYPE, pixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.WRAP_S);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.WRAP_T);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.MIN_FILTER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.MAG_FILTER);

		return id;
	}

	void Texture::bind()
	{
		glActiveTexture(GL_TEXTURE0 + m_slot);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}
}
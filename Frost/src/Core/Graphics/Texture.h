#pragma once

#include <glad/glad.h>

struct TextureConfig
{
	int INTERAL_FORMAT;
	unsigned int FORMAT;

	unsigned int TYPE;

	int WRAP_S;
	int WRAP_T;

	int MIN_FILTER;
	int MAG_FILTER;
};

class Texture
{
private:
	unsigned int m_id;
	int m_width;
	int m_height;
	int m_bpp;

	unsigned int m_slot;
public:
	Texture(const char* path, unsigned int slot = 0);
	Texture(int width, int height, unsigned int slot = 0);
	Texture(const char* path, TextureConfig config, unsigned int slot = 0);
	Texture(int width, int height, TextureConfig config, unsigned int slot = 0);
	~Texture();

	static unsigned int CreateTexture(unsigned char* pixels, int width, int height, TextureConfig config);

	void bind();

	inline unsigned int getID() const { return m_id; }
	inline unsigned int getSlot() const { return m_slot; }
};
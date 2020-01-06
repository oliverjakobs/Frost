#include "Font.h"

#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "Ignis/Packages/stb_truetype.h"

#include <fstream>

namespace ignis
{
	Font::Font(const std::string& path, float size)
	{
		m_firstChar = 32;
		m_numChars = 96; // ASCII 32..126 is 95 glyphs

		m_bitmapWidth = 512;
		m_bitmapHeight = 512;

		std::ifstream input(path, std::ios::binary);
		std::vector<GLubyte> buffer(std::istreambuf_iterator<char>(input), {});

		m_charData = (stbtt_bakedchar*)malloc(sizeof(stbtt_bakedchar) * m_numChars);

		// load bitmap
		TextureConfig config = DEFAULT_CONFIG;
		config.INTERAL_FORMAT = GL_R8;
		config.FORMAT = GL_RED;

		GLubyte* bitmap = (GLubyte*)malloc(sizeof(GLubyte) * m_bitmapWidth * m_bitmapHeight);
		stbtt_BakeFontBitmap(buffer.data(), 0, size, bitmap, m_bitmapWidth, m_bitmapHeight, m_firstChar, m_numChars, m_charData); // no guarantee this fits!

		m_texture = std::make_unique<Texture>(m_bitmapWidth, m_bitmapHeight, bitmap, config);

		free(bitmap);
	}

	Font::~Font()
	{
		free(m_charData);
	}

	void Font::Bind()
	{
		m_texture->Bind();
	}

	void Font::Unbind()
	{
		m_texture->Unbind();
	}

	std::vector<float> Font::LoadCharQuad(char c, float* x, float* y)
	{
		if ((GLchar)c >= m_firstChar && (GLchar)c < m_firstChar + m_numChars)
		{
			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(m_charData, m_bitmapWidth, m_bitmapHeight, c - m_firstChar, x, y, &q, 1);

			return 
			{
				q.x0, q.y0, q.s0, q.t0,
				q.x0, q.y1, q.s0, q.t1,
				q.x1, q.y1, q.s1, q.t1,
				q.x1, q.y0, q.s1, q.t0
			};
		}

		return {};
	}
}
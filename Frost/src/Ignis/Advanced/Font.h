#pragma once

#include "Ignis/Core/Buffer.h"
#include "Ignis/Core/Texture.h"

#include "Ignis/Packages/stb_truetype.h"

namespace ignis
{
	class Font
	{
	private:
		int m_firstChar;
		size_t m_numChars;

		int m_bitmapWidth;
		int m_bitmapHeight;

		stbtt_bakedchar* m_charData;
		
		std::unique_ptr<Texture> m_texture;

	public:
		Font(const std::string& path, float size);
		~Font();

		void Bind();
		void Unbind();

		std::vector<float> LoadCharQuad(char c, float* x, float* y);
	};
}
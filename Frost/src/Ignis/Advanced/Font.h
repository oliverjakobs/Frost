#pragma once

#include "Ignis/Core/Buffer.h"
#include "Ignis/Core/Texture.h"

#include "Ignis/Packages/stb_truetype.h"

#include <string>
#include <memory>

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
		
		ignis_texture* m_texture;

	public:
		Font(const std::string& path, float size);
		~Font();

		void Bind();
		void Unbind();

		std::vector<float> LoadCharQuad(char c, float* x, float* y);
	};
}
#pragma once

#include "BitmapFont.h"

class FontRenderer : private Singleton<FontRenderer>
{
private:
	std::map<std::string, unique_ptr<Font>> m_fonts;
public:
	static void AddFont(const std::string& name, Font* font);
	static Font* GetFont(const std::string& name);

	static void RenderText(const std::string& font, const std::string& text, float x, float y, const glm::mat4& view, const std::string& shader);
};
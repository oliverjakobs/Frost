#include "FontRenderer.h"

void FontRenderer::AddFont(const std::string& name, Font* font)
{
	Get().m_fonts[name] = unique_ptr<Font>(font);
}

Font* FontRenderer::GetFont(const std::string& name)
{
	try
	{
		return Get().m_fonts.at(name).get();
	}
	catch (std::out_of_range)
	{
		DEBUG_MESSAGE("No such font: " << name);
		return nullptr;
	}
}

void FontRenderer::RenderText(const std::string& font, const std::string& text, float x, float y, const glm::mat4& view, const std::string& shader)
{
	try
	{
		Get().m_fonts.at(font)->onRender(text, x, y, view, shader);
	}
	catch (std::out_of_range)
	{
		DEBUG_MESSAGE("No such font: " << font);
	}
}

#include "BitmapFont.h"

BitmapFont::BitmapFont(TextureAtlas* texture, float width, float height, float charSpacing)
{
	m_image = unique_ptr<Image>(new Image(texture, width, height));
	m_charSpacing = charSpacing;
}

BitmapFont::~BitmapFont()
{

}

void BitmapFont::RenderText(const std::string& text, float x, float y, const glm::mat4& view, const std::string& shader) const
{
	for (auto& c : text)
	{
		m_image->Render(x, y, c, view, shader);
		x += (m_image->GetWidth() + m_charSpacing);
	}
}

void BitmapFont::RenderText(const std::string& text, const glm::vec2& position, const glm::mat4& view, const std::string& shader) const
{
	RenderText(text, position.x, position.y, view, shader);
}

BitmapNumFont::BitmapNumFont(TextureAtlas* texture, float width, float height, float charSpacing)
{
	m_image = unique_ptr<Image>(new Image(texture, width, height));
	m_charSpacing = charSpacing;
}

BitmapNumFont::~BitmapNumFont()
{

}

void BitmapNumFont::RenderText(const std::string& text, float x, float y, const glm::mat4& view, const std::string& shader) const
{
	for (auto& c : text)
	{
		m_image->Render(x, y, c - '0', view, shader);
		x += (m_image->GetWidth() + m_charSpacing);
	}
}

void BitmapNumFont::RenderText(const std::string& text, const glm::vec2& position, const glm::mat4& view, const std::string& shader) const
{
	RenderText(text, position.x, position.y, view, shader);
}
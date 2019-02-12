#include "bitmapfont.h"

#include <string>

using namespace sb;

BitmapFont::BitmapFont(const std::string& path, float width, float height, float charSpacing)
{
	m_image = new Image(path, width, height, 8, 16);
	m_charSpacing = charSpacing;
}

BitmapFont::~BitmapFont()
{
	SAFE_DELETE(m_image);
}

void BitmapFont::onRender(const std::string& text, float x, float y, const glm::vec4& color, sb::Shader* shader) const
{
	for (auto& c : text)
	{
		m_image->renderF(x, y, c, shader);
		x += (m_image->getWidth() + m_charSpacing);
	}
}

void BitmapFont::onRender(const std::string& text, const glm::vec2& position, const glm::vec4& color, sb::Shader* shader) const
{
	onRender(text, position.x, position.y, color, shader);
}

BitmapNumFont::BitmapNumFont(const std::string& path, float width, float height, float charSpacing)
{
	m_image = new Image(path, width, height, 1, 11);
	m_charSpacing = charSpacing;
}

BitmapNumFont::~BitmapNumFont()
{
	SAFE_DELETE(m_image);
}

void BitmapNumFont::onRender(const std::string& text, float x, float y, const glm::vec4& color, sb::Shader* shader) const
{
	for (auto& c : text)
	{
		m_image->renderF(x, y, c - '0', shader);
		x += (m_image->getWidth() + m_charSpacing);
	}
}

void BitmapNumFont::onRender(const std::string& text, const glm::vec2& position, const glm::vec4& color, sb::Shader* shader) const
{
	onRender(text, position.x, position.y, color, shader);
}
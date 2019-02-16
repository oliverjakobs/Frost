#include "BitmapFont.h"

using namespace sb;

BitmapFont::BitmapFont(const std::string& path, float width, float height, float charSpacing)
{
	m_image = shared_ptr<Image>(new Image(path, width, height, 8, 16));
	m_charSpacing = charSpacing;
}

BitmapFont::~BitmapFont()
{

}

void BitmapFont::onRender(const std::string& text, float x, float y, const glm::mat4& view, const std::string& shader) const
{
	for (auto& c : text)
	{
		m_image->renderF(x, y, c, view, shader);
		x += (m_image->getWidth() + m_charSpacing);
	}
}

void BitmapFont::onRender(const std::string& text, const glm::vec2& position, const glm::mat4& view, const std::string& shader) const
{
	onRender(text, position.x, position.y, view, shader);
}

BitmapNumFont::BitmapNumFont(const std::string& path, float width, float height, float charSpacing)
{
	m_image = shared_ptr<Image>(new Image(path, width, height, 1, 11));
	m_charSpacing = charSpacing;
}

BitmapNumFont::~BitmapNumFont()
{

}

void BitmapNumFont::onRender(const std::string& text, float x, float y, const glm::mat4& view, const std::string& shader) const
{
	for (auto& c : text)
	{
		m_image->renderF(x, y, c - '0', view, shader);
		x += (m_image->getWidth() + m_charSpacing);
	}
}

void BitmapNumFont::onRender(const std::string& text, const glm::vec2& position, const glm::mat4& view, const std::string& shader) const
{
	onRender(text, position.x, position.y, view, shader);
}
#pragma once

#include "Font.h"

class BitmapFont : public Font
{
private:
	unique_ptr<Image> m_image;

	float m_charSpacing;
public:
	BitmapFont(TextureAtlas* texture, float width, float height, float charSpacing);
	~BitmapFont();

	void RenderText(const std::string& text, float x, float y, const glm::mat4& view, const std::string& shader) const override;
	void RenderText(const std::string& text, const glm::vec2& position, const glm::mat4& view, const std::string& shader) const override;
};

class BitmapNumFont : public Font
{
private:
	unique_ptr<Image> m_image;

	float m_charSpacing;
public:
	BitmapNumFont(TextureAtlas* texture, float width, float height, float charSpacing);
	~BitmapNumFont();

	void RenderText(const std::string& text, float x, float y, const glm::mat4& view, const std::string& shader) const override;
	void RenderText(const std::string& text, const glm::vec2& position, const glm::mat4& view, const std::string& shader) const override;
};
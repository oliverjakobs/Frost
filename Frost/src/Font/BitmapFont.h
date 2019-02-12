#pragma once

#include "Font.h"

class BitmapFont : public Font
{
private:
	sb::Image* m_image;

	float m_charSpacing;
public:
	BitmapFont(const std::string& path, float width, float height, float charSpacing);
	~BitmapFont();

	void onRender(const std::string& text, float x, float y, const glm::vec4& color, sb::Shader* shader) const override;
	void onRender(const std::string& text, const glm::vec2& position, const glm::vec4& color, sb::Shader* shader) const override;
};

class BitmapNumFont : public Font
{
private:
	sb::Image* m_image;

	float m_charSpacing;
public:
	BitmapNumFont(const std::string& path, float width, float height, float charSpacing);
	~BitmapNumFont();

	void onRender(const std::string& text, float x, float y, const glm::vec4& color, sb::Shader* shader) const override;
	void onRender(const std::string& text, const glm::vec2& position, const glm::vec4& color, sb::Shader* shader) const override;
};
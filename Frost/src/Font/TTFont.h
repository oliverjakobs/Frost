#pragma once

#include "Font.h"

class TTFont : public Font
{
private:
public:
	TTFont(const std::string& path); 
	~TTFont();

	void RenderText(const std::string& text, float x, float y, const glm::mat4& view, const std::string& shader) const override;
	void RenderText(const std::string& text, const glm::vec2& position, const glm::mat4& view, const std::string& shader) const override;
};
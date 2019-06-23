#pragma once

#include "Core/Graphics/Image.h"

class Font
{
public:
	virtual void onRender(const std::string& text, float x, float y, const glm::mat4& view, const std::string& shader) const = 0;
	virtual void onRender(const std::string& text, const glm::vec2& position, const glm::mat4& view, const std::string& shader) const = 0;
};
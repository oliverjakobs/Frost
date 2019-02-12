#pragma once

#include "Scrapbook/Scrapbook.h"

class Font
{
public:
	virtual void onRender(const std::string& text, float x, float y, const glm::vec4& color, sb::Shader* shader) const = 0;
	virtual void onRender(const std::string& text, const glm::vec2& position, const glm::vec4& color, sb::Shader* shader) const = 0;
};
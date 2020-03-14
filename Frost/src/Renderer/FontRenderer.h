#pragma once

#include "RenderState.h"

#include <string>

struct FontRenderer
{
	static void Init(ignis_shader* shader);
	static void Destroy();

	static void AddFont(const std::string& name, ignis_font* font);
	static ignis_font* GetFont(const std::string& name);

	static void RenderText(const std::string& font, const std::string& text, float x, float y, const glm::mat4& proj, const color& color);
};

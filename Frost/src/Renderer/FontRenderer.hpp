#pragma once

#include "Ignis/Ignis.h"

#include <glm/glm.hpp>

struct FontRenderer
{
	static void Init(ignis_shader* shader);
	static void Destroy();

	static void RenderText(ignis_font* font, const char* text, float x, float y, const glm::mat4& proj, const ignis_color_rgba& color);
};
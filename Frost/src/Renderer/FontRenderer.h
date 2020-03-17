#pragma once

#include "RenderState.h"

struct FontRenderer
{
	static void Init(ignis_shader* shader);
	static void Destroy();

	static void RenderText(ignis_font* font, const char* text, float x, float y, const glm::mat4& proj, const color& color);
};
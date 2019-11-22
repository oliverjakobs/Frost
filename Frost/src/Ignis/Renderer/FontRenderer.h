#pragma once

#include "Ignis/Font.h"
#include "Ignis/Core/Shader.h"

namespace ignis
{
	struct FontRenderer
	{
		static void Init(const std::shared_ptr<Shader>& shader);
		static void Destroy();

		static void RenderText(Font& font, const std::string& text, float x, float y, const glm::mat4& proj, const ignis::color& color);
	};
}

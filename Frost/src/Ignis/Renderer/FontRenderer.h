#pragma once

#include "Ignis/Advanced/Font.h"
#include "Ignis/Core/Shader.h"
#include "Ignis/Color.h"

namespace ignis
{
	struct FontRenderer
	{
		static void Init(ignis_shader* shader);
		static void Destroy();

		static void AddFont(const std::string& name, std::shared_ptr<Font> font);
		static std::shared_ptr<Font> GetFont(const std::string& name);

		static void RenderText(const std::string& font, const std::string& text, float x, float y, const glm::mat4& proj, const color& color);
	};
}

#pragma once

#include "Ignis/Advanced/Font.h"
#include "Ignis/Core/Shader.h"

namespace ignis
{
	struct FontRenderer
	{
		static void Init(const std::shared_ptr<Shader>& shader);
		static void Destroy();

		static void AddFont(const std::string& name, std::shared_ptr<Font> font);
		static std::shared_ptr<Font> GetFont(const std::string& name);

		static void RenderText(Font& font, const std::string& text, float x, float y, const glm::mat4& proj, const ignis::color& color);
		static void RenderText(const std::string& font, const std::string& text, float x, float y, const glm::mat4& proj, const ignis::color& color);
	};
}

#pragma once

#include "Ignis/Core/Buffer.h"
#include "Ignis/Core/Shader.h"
#include "Ignis/Core/Texture.h"

namespace ignis
{
	struct Renderer2D
	{
		static void Init(const std::shared_ptr<Shader>& shader);
		static void Destroy();

		static void Start(const glm::mat4& viewProjection);
		static void Flush();

		static void RenderQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		// Textures
		static void RenderQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture);
		static void RenderTexture(const std::shared_ptr<Texture>& texture, const glm::mat4& model, const glm::mat4& src);
	};
}

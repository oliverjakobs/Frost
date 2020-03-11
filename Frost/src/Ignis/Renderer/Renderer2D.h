#pragma once

#include "Ignis/Core/Buffer.h"
#include "Ignis/Core/Shader.h"
#include "Ignis/Core/Texture.h"

#include "Ignis/Color.h"

namespace ignis
{
	struct Renderer2D
	{
		static void Init(ignis_shader* shader);
		static void Destroy();

		static void Start(const glm::mat4& viewProjection);
		static void Flush();

		// Textures
		static void RenderTexture(const std::shared_ptr<Texture>& texture, const glm::mat4& model, const glm::mat4& src, const color& color = WHITE);
	};
}

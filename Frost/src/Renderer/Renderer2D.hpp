#pragma once

#include "Ignis/Ignis.h"

#include <glm/glm.hpp>

struct Renderer2D
{
	static void Init(ignis_shader* shader);
	static void Destroy();

	static void Start(const glm::mat4& viewProjection);
	static void Flush();

	// Textures
	static void RenderTexture(ignis_texture* texture, const glm::mat4& model, const glm::mat4& src, const ignis_color_rgba& color = IGNIS_WHITE);
};

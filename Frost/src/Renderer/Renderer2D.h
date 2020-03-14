#pragma once

#include "RenderState.h"

struct Renderer2D
{
	static void Init(ignis_shader* shader);
	static void Destroy();

	static void Start(const glm::mat4& viewProjection);
	static void Flush();

	// Textures
	static void RenderTexture(ignis_texture* texture, const glm::mat4& model, const glm::mat4& src, const color& color = WHITE);
};

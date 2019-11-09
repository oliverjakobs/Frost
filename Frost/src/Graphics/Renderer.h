#pragma once

#include "ResourceManager.h"
#include "Primitives.h"

#include "View/View.h"
#include "Maths/Maths.h"

struct Renderer
{
	static void EnableBlend(GLenum sfactor, GLenum dfactor);
	static void SetClearColor(float r, float g, float b, float a);
	static void SetClearColor(const ignis::color& color);

	// ----------------------------texture---------------------------------------------------------------------------
	static void RenderTexture(Texture* tex);
	static void RenderTextureInstanced(Texture* tex, uint instanceCount);
};
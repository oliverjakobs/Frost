#pragma once

#include "ResourceManager.h"
#include "Primitives.h"

#include "View/View.h"
#include "Maths/Maths.h"

const Color BLACK =		Color(0.0f, 0.0f, 0.0f, 1.0f);
const Color WHITE =		Color(1.0f, 1.0f, 1.0f, 1.0f);
const Color RED =		Color(1.0f, 0.0f, 0.0f, 1.0f);
const Color GREEN =		Color(0.0f, 1.0f, 0.0f, 1.0f);
const Color BLUE =		Color(0.0f, 0.0f, 1.0f, 1.0f);
const Color YELLOW =	Color(1.0f, 1.0f, 0.0f, 1.0f);
const Color MAGENTA =	Color(1.0f, 0.0f, 1.0f, 1.0f);
const Color CYAN =		Color(0.0f, 1.0f, 1.0f, 1.0f);

inline Color blendColor(const Color& color, float alpha)
{
	return Color(color.r, color.g, color.b, alpha);
}

class Renderer : private Singleton<Renderer>
{
public:
	static void Start();
	static void Flush();

	static void EnableBlend(GLenum sfactor, GLenum dfactor);
	static void SetClearColor(float r, float g, float b, float a);
	static void SetClearColor(const Color& color);

	// ----------------------------texture---------------------------------------------------------------------------
	static void RenderTexture(Texture* tex);
	static void RenderTextureInstanced(Texture* tex, uint instanceCount);
};
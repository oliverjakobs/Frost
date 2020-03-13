#pragma once

// Core
#include "Core/VertexArray.hpp"
#include "Core/Texture.h"
#include "Core/Shader.h"

#include "Advanced/Font.h"

#include "Ignis/Camera/OrthographicCamera.h"

#include "Renderer/RenderState.h"

// Renderer
#include "Renderer/FontRenderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Primitives.h"

namespace ignis
{
	bool ignisInit(int width, int height);
	bool ignisLoadGL(bool debug = false);
	void ignisViewport(int x, int y, int w, int h);

	const glm::mat4& ignisScreenMat();
}
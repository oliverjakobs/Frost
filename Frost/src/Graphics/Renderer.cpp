#include "Renderer.h"

void Renderer::EnableBlend(GLenum sfactor, GLenum dfactor)
{
	glEnable(GL_BLEND);
	glBlendFunc(sfactor, dfactor);
}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void Renderer::SetClearColor(const Color& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::RenderTexture(Texture* tex)
{
	if (tex != nullptr)
	{
		tex->Bind();
		std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
	}
}

void Renderer::RenderTextureInstanced(Texture* tex, uint instanceCount)
{
	if (tex != nullptr)
	{
		tex->Bind();
		std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };

		glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data(), instanceCount);
	}
}
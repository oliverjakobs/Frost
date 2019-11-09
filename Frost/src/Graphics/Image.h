#pragma once

#include "Types.h"

#include "Ignis/Advanced/TextureAtlas.h"
#include "Ignis/Core/Buffer.h"
#include "Ignis/Core/Shader.h"

class Image
{
private:
	ignis::TextureAtlas* m_texture;
	ignis::VertexArray m_vertexArray;

	RenderFlip m_flip;

	float m_width;
	float m_height;

public:
	Image(ignis::TextureAtlas* texture, float width, float height);
	~Image();

	void SetRenderFlip(RenderFlip flip);

	void Render(float x, float y, const glm::mat4& view, const std::string& shader) const;
	void Render(const glm::vec2& pos, const glm::mat4& view, const std::string& shader) const;

	void Render(float x, float y, int frame, const glm::mat4& view, const std::string& shader) const;
	void Render(const glm::vec2& pos, int frame, const glm::mat4& view, const std::string& shader) const;

	inline float GetWidth() const { return m_width; }
	inline float GetHeight() const { return m_height; }
};
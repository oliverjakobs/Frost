#pragma once

#include "Component.hpp"
#include "IgnisRenderer/IgnisRenderer.h"

enum class RenderFlip
{
	NONE = 0, 
	HORIZONTAL = 1, 
	VERTICAL = 2,
	BOTH = 3
};

class TextureComponent : public Component
{
private:
	ignis_texture* m_texture;
	size_t m_frame;

	float m_width;
	float m_height;

	RenderFlip m_renderFlip;

public:
	TextureComponent(Entity* entity, ignis_texture* texture, float width, float height);
	~TextureComponent();

	void SetFrame(size_t frame);
	void SetRenderFlip(RenderFlip flip);

	float GetWidth() const { return m_width; }
	float GetHeight() const { return m_height; }
	glm::vec2 GetDimension() const { return glm::vec2(m_width, m_height); }

	void OnRender(Scene* scene) override;
};
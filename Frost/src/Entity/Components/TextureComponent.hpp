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
	IgnisTexture* m_texture;
	size_t m_frame;

	float m_width;
	float m_height;

	RenderFlip m_renderFlip;

public:
	TextureComponent(Entity* entity, IgnisTexture* texture, float width, float height, size_t frame = 0);
	~TextureComponent();

	void SetFrame(size_t frame);
	void SetRenderFlip(RenderFlip flip);

	float GetWidth() const { return m_width; }
	float GetHeight() const { return m_height; }
	vec2 GetDimension() const { return vec2_(m_width, m_height); }

	void OnRender(Scene* scene) override;
};
#pragma once

#include "Component.h"
#include "Ignis/Ignis.h"

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
	std::shared_ptr<ignis::Texture> m_texture;
	size_t m_frame;

	float m_width;
	float m_height;

	RenderFlip m_renderFlip;

public:
	TextureComponent(Entity* entity, std::shared_ptr<ignis::Texture> texture, float width, float height);

	void SetFrame(size_t frame);
	void SetRenderFlip(RenderFlip flip);

	void OnRender(Scene* scene) override;
};
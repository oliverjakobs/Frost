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
	uint m_frame;

	float m_width;
	float m_height;

	RenderFlip m_renderFlip;

public:
	TextureComponent(std::shared_ptr<ignis::Texture> texture, float width, float height);

	void SetFrame(uint frame);
	void SetRenderFlip(RenderFlip flip);

	void OnUpdate(float deltaTime) override;
	void OnRender() override;
};
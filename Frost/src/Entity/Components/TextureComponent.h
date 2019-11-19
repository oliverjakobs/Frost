#pragma once

#include "Component.h"
#include "Ignis/Ignis.h"

class TextureComponent : public Component
{
private:
	unique_ptr<ignis::Texture> m_image;
	uint m_frame;

public:
	TextureComponent(ignis::Texture* image);

	void SetFrame(uint frame);
	void SetRenderFlip(RenderFlip flip);

	void OnUpdate(float deltaTime) override;
	void OnRender() override;
};
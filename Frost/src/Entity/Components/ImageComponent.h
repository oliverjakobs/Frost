#pragma once

#include "Component.h"
#include "Graphics.h"

class ImageComponent : public Component
{
private:
	unique_ptr<Image> m_image;
	uint m_frame;

public:
	ImageComponent(Image* image);

	void SetFrame(uint frame);
	void SetRenderFlip(RenderFlip flip);

	void OnUpdate(float deltaTime) override;
	void OnRender() override;
};
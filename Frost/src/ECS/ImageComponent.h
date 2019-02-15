#pragma once

#include "Scrapbook/Graphics.h"
#include "Component.h"

class ImageComponent : public Component
{
private:
	shared_ptr<sb::Image> m_sprite;
public:
	ImageComponent(shared_ptr<sb::Image> sprite);
	~ImageComponent();

	ImageComponent* clone();

	void onInput() override;
	void onUpdate() override;
	void onRender() const override;
};
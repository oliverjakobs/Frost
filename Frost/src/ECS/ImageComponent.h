#pragma once

#include "Scrapbook/Graphics.h"
#include "Component.h"

class ImageComponent : public Component
{
private:
	sb::Image* m_sprite;
public:
	ImageComponent(sb::Image* sprite);
	~ImageComponent();
	
	void onInput() override;
	void onUpdate() override;
	void onRender() const override;
};
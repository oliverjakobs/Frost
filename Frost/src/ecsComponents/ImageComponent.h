#pragma once

#include "ecs/ecsRegistry.h"
#include "Core/Graphics.h"

class ImageComponent : public ecs::Component
{
public:
	GENERATE_COMPONENT_TYPE_ID(ImageComponent)

	Image* m_image;

	ImageComponent() : m_image(nullptr) {}
	virtual ~ImageComponent();

	virtual bool init(void* desc) override final;

	GCC_MEMORYPOOL_DECLARATION(32);
};
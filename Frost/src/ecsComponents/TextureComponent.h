#pragma once

#include "ecs/ecs.h"
#include "Core/Graphics.h"

class TextureComponent : public ecsComponent
{
public:
	GENERATE_COMPONENT_TYPE_ID(TextureComponent)

	Texture* m_texture;

	TextureComponent() : m_texture(nullptr) {}
	virtual ~TextureComponent();

	virtual bool init(void* desc) override final;

	GCC_MEMORYPOOL_DECLARATION(32);
};
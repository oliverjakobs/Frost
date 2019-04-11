#include "TextureComponent.h"

#include "Log/Logger.h"
#include "Utility/Utils.h"

GCC_MEMORYPOOL_DEFINITION(TextureComponent);
TextureComponent::~TextureComponent()
{
	SAFE_DELETE(m_texture);
}

bool TextureComponent::init(void* desc)
{
	return false;
}

#include "ImageComponent.h"

GCC_MEMORYPOOL_DEFINITION(ImageComponent);
ImageComponent::~ImageComponent()
{
	//SAFE_DELETE(m_image);
}

bool ImageComponent::init(void* desc)
{
	return false;
}

#include "ImageComponent.h"

ImageComponent::ImageComponent(shared_ptr<sb::Image> sprite)
{
	m_sprite = sprite;
}

ImageComponent::~ImageComponent()
{

}

ImageComponent* ImageComponent::clone()
{
	return new ImageComponent(*this);
}

void ImageComponent::onInput()
{
}

void ImageComponent::onUpdate()
{
}

void ImageComponent::onRender() const
{
	float x = m_entity->getPosition().x - m_sprite->getWidth() / 2.0f;
	float y = m_entity->getPosition().y;

	if (m_sprite != nullptr)
		m_sprite->render(x, y, sb::Renderer::GetViewMat(), "shader");
}
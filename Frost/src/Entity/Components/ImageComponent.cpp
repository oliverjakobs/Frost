#include "ImageComponent.h"

ImageComponent::ImageComponent(Image* image)
	: m_image(image), m_frame(0) 
{
	
}

void ImageComponent::SetFrame(uint frame)
{
	m_frame = frame;
}

void ImageComponent::SetRenderFlip(RenderFlip flip)
{
	m_image->setRenderFlip(flip);
}

void ImageComponent::OnUpdate()
{
}

void ImageComponent::OnRender()
{
	float x = m_entity->GetPosition().x - (m_image->getWidth() / 2.0f);
	float y = m_entity->GetPosition().y;
	m_image->renderF(x, y, m_frame, Renderer::GetViewMat(), "shader");
}

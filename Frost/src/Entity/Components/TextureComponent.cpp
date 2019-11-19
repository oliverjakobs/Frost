#include "TextureComponent.h"

TextureComponent::TextureComponent(ignis::Texture* image) : m_image(image), m_frame(0)
{
	
}

void TextureComponent::SetFrame(uint frame)
{
	m_frame = frame;
}

void TextureComponent::SetRenderFlip(RenderFlip flip)
{
	//m_image->SetRenderFlip(flip);
}

void TextureComponent::OnUpdate(float deltaTime)
{
}

void TextureComponent::OnRender()
{
	float x = m_entity->GetPosition().x - (m_image->GetWidth() / 2.0f);
	float y = m_entity->GetPosition().y;
	
	//m_image->Render(x, y, m_frame, View::GetMat(), "shader");
}

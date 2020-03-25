#include "TextureComponent.hpp"

#include <glm/gtc/matrix_transform.hpp>

TextureComponent::TextureComponent(Entity* entity, IgnisTexture* texture, float width, float height, size_t frame)
	: Component(entity), m_texture(texture), m_frame(frame), m_width(width), m_height(height), m_renderFlip(RenderFlip::NONE)
{

}

TextureComponent::~TextureComponent()
{

}

void TextureComponent::SetFrame(size_t frame)
{
	m_frame = frame;
}

void TextureComponent::SetRenderFlip(RenderFlip flip)
{
	m_renderFlip = flip;
}

void TextureComponent::OnRender(Scene* scene)
{
	float x = m_entity->GetPosition().x - m_width / 2.0f;
	float y = m_entity->GetPosition().y;

	float src_w = 1.0f / m_texture->columns;
	float src_h = 1.0f / m_texture->rows;

	float src_x = (m_frame % m_texture->columns) * src_w;
	float src_y = 1 - src_h - ((m_frame / m_texture->columns) * src_h);

	switch (m_renderFlip)
	{
	case RenderFlip::NONE:
		BatchRenderer2DRenderTextureFrame(m_texture, x, y, m_width, m_height, src_x, src_y, src_w, src_h);
		break;
	case RenderFlip::HORIZONTAL:
		BatchRenderer2DRenderTextureFrame(m_texture, x, y, m_width, m_height, src_x + src_w, src_y, -src_w, src_h);
		break;
	case RenderFlip::VERTICAL:
		BatchRenderer2DRenderTextureFrame(m_texture, x, y, m_width, m_height, src_x, src_y + src_h, src_w, -src_h);
		break;
	case RenderFlip::BOTH:
		BatchRenderer2DRenderTextureFrame(m_texture, x, y, m_width, m_height, src_x + src_w, src_y + src_h, -src_w, -src_h);
		break;
	}
}

#include "TextureComponent.h"

#include <glm/gtc/matrix_transform.hpp>

TextureComponent::TextureComponent(Entity* entity, std::shared_ptr<ignis::Texture> texture, float width, float height)
	: Component(entity), m_texture(texture), m_frame(0), m_width(width), m_height(height), m_renderFlip(RenderFlip::NONE)
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

void TextureComponent::OnUpdate(float deltaTime)
{
}

void TextureComponent::OnRender()
{
	float x = m_entity->GetPosition().x - m_width / 2.0f;
	float y = m_entity->GetPosition().y;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(x, y, 0.0f));
	model = glm::scale(model, glm::vec3(m_width, m_height, 1.0f));

	float src_w = 1.0f / m_texture->GetColumns();
	float src_h = 1.0f / m_texture->GetRows();

	float src_x = (m_frame % m_texture->GetColumns()) * src_w;
	float src_y = 1 - src_h - ((m_frame / m_texture->GetColumns()) * src_h);

	glm::mat4 src = glm::mat4(1.0f);

	switch (m_renderFlip)
	{
	case RenderFlip::NONE:
		src = glm::translate(src, glm::vec3(src_x, src_y, 0.0f));
		src = glm::scale(src, glm::vec3(src_w, src_h, 1.0f));
		break;
	case RenderFlip::HORIZONTAL:
		src = glm::translate(src, glm::vec3(src_x + src_w, src_y, 0.0f));
		src = glm::scale(src, glm::vec3(-src_w, src_h, 1.0f));
		break;
	case RenderFlip::VERTICAL:
		src = glm::translate(src, glm::vec3(src_x, src_y + src_h, 0.0f));
		src = glm::scale(src, glm::vec3(src_w, -src_h, 1.0f));
		break;
	case RenderFlip::BOTH:
		src = glm::translate(src, glm::vec3(src_x + src_w, src_y + src_h, 0.0f));
		src = glm::scale(src, glm::vec3(-src_w, -src_h, 1.0f));
		break;
	}

	ignis::Renderer2D::RenderTexture(m_texture, model, src);
}

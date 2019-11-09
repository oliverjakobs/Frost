#include "Image.h"

#include "Renderer.h"

using namespace ignis;

Image::Image(TextureAtlas* texture, float width, float height)
	: m_texture(texture), m_width(width), m_height(height), m_flip(RenderFlip::NONE)
{
	float fWidth = 1.0f / m_texture->GetColumns();
	float fHeight = 1.0f / m_texture->GetRows();

	GLfloat vertices[] = 
	{
		 0.0f,   0.0f,   0.0f,    0.0f,
		width,   0.0f, fWidth,    0.0f,
		width, height, fWidth, fHeight,
		 0.0f, height,   0.0f, fHeight
	};

	m_vertexArray.AddArrayBuffer(make_shared<ArrayBuffer>(sizeof(vertices), vertices, GL_STATIC_DRAW),
		{
			{GL_FLOAT, 2}, {GL_FLOAT, 2}
		});
}

Image::~Image()
{

}

void Image::SetRenderFlip(RenderFlip flip)
{
	m_flip = flip;
}

void Image::Render(float x, float y, const glm::mat4& view, const std::string& shader) const
{
	Render(glm::vec2(x, y), 0, view, shader);
}

void Image::Render(const glm::vec2& pos, const glm::mat4& view, const std::string& shader) const
{
	Render(pos, 0, view, shader);
}

void Image::Render(float x, float y, int frame, const glm::mat4& view, const std::string& shader) const
{
	Render(glm::vec2(x, y), frame, view, shader);
}

void Image::Render(const glm::vec2& pos, int frame, const glm::mat4& view, const std::string& sName) const
{
	Shader* shader = ResourceManager::GetShader(sName);

	if (shader != nullptr)
	{
		shader->Use();

		shader->SetUniform1i("uFrame", frame);

		shader->SetUniform1i("uRenderFlip", (int)m_flip);
		shader->SetUniform1i("uRows", m_texture->GetRows());
		shader->SetUniform1i("uColumns", m_texture->GetColumns());

		shader->SetUniformMat4("projection", glm::mat4());
		shader->SetUniformMat4("view", view);
		shader->SetUniformMat4("model", glm::translate(glm::mat4(), glm::vec3(pos, 0.0f)));
	}

	m_vertexArray.Bind();

	Renderer::RenderTexture((ignis::Texture*)m_texture);
}
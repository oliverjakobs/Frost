#include "Image.h"

#include "Renderer.h"

Image::Image(const std::string& src, float width, float height, uint rows, uint columns)
	: m_width(width), m_height(height)
{
	float fWidth = 1.0f / columns;
	float fHeight = 1.0f / rows;

	GLfloat vertices[] = 
	{
		 0.0f,   0.0f,   0.0f,    0.0f,
		width,   0.0f, fWidth,    0.0f,
		width, height, fWidth, fHeight,
		 0.0f, height,   0.0f, fHeight
	};

	m_flip = FLIP_NONE;

	m_vao.Bind();

	m_vao.BindVertexBuffer();

	m_vao.SetVertexBufferData(sizeof(vertices), vertices);
	m_vao.SetVertexAttribPointer(0, 2, 4, 0);
	m_vao.SetVertexAttribPointer(1, 2, 4, 2);

	m_vao.UnbindVertexBuffer();

	m_texture = unique_ptr<TextureAtlas>(new TextureAtlas(src.c_str(), rows, columns));
}

Image::~Image()
{

}

void Image::setRenderFlip(RenderFlip flip)
{
	m_flip = flip;
}

void Image::render(float x, float y, const glm::mat4& view, const std::string& shader) const
{
	renderF(glm::vec2(x, y), 0, view, shader);
}

void Image::render(const glm::vec2& pos, const glm::mat4& view, const std::string& shader) const
{
	renderF(pos, 0, view, shader);
}

void Image::renderF(float x, float y, int frame, const glm::mat4& view, const std::string& shader) const
{
	renderF(glm::vec2(x, y), frame, view, shader);
}

void Image::renderF(const glm::vec2& pos, int frame, const glm::mat4& view, const std::string& sName) const
{
	Shader* shader = ResourceManager::GetShader(sName);

	if (shader != nullptr)
	{
		shader->use();

		shader->setUniform1i("uFrame", frame);

		shader->setUniform1i("uRenderFlip", (int)m_flip);
		shader->setUniform1i("uRows", m_texture->rows);
		shader->setUniform1i("uColumns", m_texture->columns);

		shader->setUniformMat4("projection", glm::mat4());
		shader->setUniformMat4("view", view);
		shader->setUniformMat4("model", glm::translate(glm::mat4(), glm::vec3(pos, 0.0f)));
	}

	m_vao.Bind();

	Renderer::RenderTexture(m_texture.get());

	m_vao.Unbind();
}
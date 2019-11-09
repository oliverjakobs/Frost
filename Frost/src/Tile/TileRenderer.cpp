#include "TileRenderer.h"

#include "Graphics/Renderer.h"

TileRenderer::TileRenderer(ignis::TextureAtlas* textureAtlas, const std::vector<Tile>& tiles, float size)
	: m_texture(textureAtlas)
{
	std::vector<glm::vec2> translations;
	std::vector<GLuint> frames;

	for (auto& tile : tiles)
	{
		translations.push_back(tile.position);
		frames.push_back(tile.id);
	}

	m_instanceCount = tiles.size();

	float fWidth = (textureAtlas->GetColumns() > 0) ? 1.0f / textureAtlas->GetColumns() : 1.0f;
	float fHeight = (textureAtlas->GetRows() > 0) ? 1.0f / textureAtlas->GetRows() : 1.0f;

	float vertices[] =
	{
		// positions	// texcoords
		0.0f, 0.0f,		0.0f, 0.0f,
		size, 0.0f,		fWidth, 0.0f,
		size, size,		fWidth, fHeight,
		0.0f, size,		0.0f, fHeight
	};

	m_vao.AddArrayBuffer(make_shared<ignis::ArrayBuffer>(sizeof(vertices), vertices),
	{
		{GL_FLOAT, 2}, { GL_FLOAT, 2 }
	});

	// also set instance data
	m_vao.AddArrayBuffer(make_shared<ignis::ArrayBuffer>(sizeof(glm::vec2) * translations.size(), &translations[0]),
	{
		{GL_FLOAT, 2}
	});
	glVertexAttribDivisor(2, 1);

	m_vao.AddArrayBuffer(make_shared<ignis::ArrayBuffer>(sizeof(GLuint) * frames.size(), &frames[0]),
	{
		{GL_UNSIGNED_INT, 1}
	});
	glVertexAttribDivisor(3, 1);

}

void TileRenderer::RenderMap(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{		
	ignis::Shader* shader = ResourceManager::GetShader("instanced");

	if (shader != nullptr)
	{
		shader->Use();

		shader->SetUniform1i("uRows", m_texture->GetRows());
		shader->SetUniform1i("uColumns", m_texture->GetColumns());
		
		shader->SetUniformMat4("projection", projection);
		shader->SetUniformMat4("view", view);
		shader->SetUniformMat4("model", model);
	}

	m_vao.Bind();

	Renderer::RenderTextureInstanced(m_texture, m_instanceCount);

	m_vao.Unbind();
}

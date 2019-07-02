#include "TileRenderer.h"

TileRenderer::TileRenderer(TextureAtlas* textureAtlas, const std::vector<Tile>& tiles, float size)
{
	std::vector<glm::vec2> translations;
	std::vector<GLuint> frames;

	for (auto& tile : tiles)
	{
		translations.push_back(tile.position);
		frames.push_back(tile.id);
	}

	m_instanceCount = tiles.size();

	float fWidth = (textureAtlas->columns > 0) ? 1.0f / textureAtlas->columns : 1.0f;
	float fHeight = (textureAtlas->rows > 0) ? 1.0f / textureAtlas->rows : 1.0f;

	float vertices[] =
	{
		// positions	// texcoords
		0.0f, 0.0f,		0.0f, 0.0f,
		size, 0.0f,		fWidth, 0.0f,
		size, size,		fWidth, fHeight,
		0.0f, size,		0.0f, fHeight
	};

	m_vao.Bind();

	m_vao.BindVertexBuffer();
	m_vao.SetVertexBufferData(sizeof(vertices), vertices);
	m_vao.SetVertexAttribPointer(0, 2, 4, 0);
	m_vao.SetVertexAttribPointer(1, 2, 4, 2);

	// also set instance data
	m_vao.BindVertexBuffer();
	m_vao.SetVertexBufferData(sizeof(glm::vec2) * translations.size(), &translations[0]);
	m_vao.SetVertexAttribPointer(2, 2, 2, 0);
	m_vao.SetVertexAttribDivisor(2, 1);
	
	m_vao.BindVertexBuffer();
	m_vao.SetVertexBufferData(sizeof(GLuint) * frames.size(), &frames[0]);
	m_vao.SetVertexAttribIPointer(3, 1, 1, 0);
	m_vao.SetVertexAttribDivisor(3, 1);

	m_vao.UnbindVertexBuffer();

	m_texture = unique_ptr<TextureAtlas>(textureAtlas);
}

void TileRenderer::RenderMap(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{		
	Shader* shader = ResourceManager::GetShader("instanced");

	if (shader != nullptr)
	{
		shader->use();

		shader->setUniform1i("uRows", m_texture->rows);
		shader->setUniform1i("uColumns", m_texture->columns);
		
		shader->setUniformMat4("projection", projection);
		shader->setUniformMat4("view", view);
		shader->setUniformMat4("model", model);
	}

	m_vao.Bind();

	Renderer::RenderTextureInstanced(m_texture.get(), m_instanceCount);

	m_vao.Unbind();
}

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

	float quadVertices[] = {
		// positions      // texcoords
		0.0f, 0.0f, 0.0f, 0.0f,
		size, 0.0f,	fWidth, 0.0f,
		size, size,	fWidth, fHeight,
		0.0f, size, 0.0f, fHeight
	};

	m_vao.Bind();

	m_vao.BindVertexBuffer();
	m_vao.SetVertexBufferData(sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	m_vao.SetVertexAttribPointer(0, 2, 4, 0);
	m_vao.SetVertexAttribPointer(1, 2, 4, 2);

	// also set instance data
	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * translations.size(), &translations[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glVertexAttribDivisor(2, 1); 

	unsigned int frameVBO;
	glGenBuffers(1, &frameVBO);
	glBindBuffer(GL_ARRAY_BUFFER, frameVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * frames.size(), &frames[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, sizeof(GLuint), (void*)0);
	glVertexAttribDivisor(3, 1); 

	m_vao.UnbindVertexBuffer();

	texture = unique_ptr<TextureAtlas>(textureAtlas);
}

void TileRenderer::RenderMap()
{		
	Shader* shader = ResourceManager::GetShader("instanced");

	if (shader != nullptr)
	{
		shader->use();

		shader->setUniform1i("uRows", texture->rows);
		shader->setUniform1i("uColumns", texture->columns);
		
		shader->setUniformMat4("projection", glm::mat4());
		shader->setUniformMat4("view", Renderer::GetViewMat());
		shader->setUniformMat4("model", glm::mat4());
	}

	texture->Bind();

	std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };

	m_vao.Bind();
	glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data(), m_instanceCount);
	glBindVertexArray(0);
}

void TileRenderer::RenderMap(const std::vector<Tile>& tiles, Image* image)
{
}

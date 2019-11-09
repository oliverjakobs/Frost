#pragma once

#include "Tile.h"

#include "Graphics/ResourceManager.h"
#include "Ignis/Core/Buffer.h"

class TileRenderer
{
private:
	ignis::VertexArray m_vao;

	uint m_instanceCount;

	ignis::TextureAtlas* m_texture;
public:
	TileRenderer(ignis::TextureAtlas* textureAtlas, const std::vector<Tile>& tiles, float tileSize);

	void RenderMap(const glm::mat4& model, const glm::mat4& view, const glm::mat4& perspective);
};
#pragma once

#include "Tile.h"

#include "Graphics.h"

class TileRenderer
{
private:
	VAO m_vao;

	uint m_instanceCount;

	TextureAtlas* m_texture;
public:
	TileRenderer(TextureAtlas* textureAtlas, const std::vector<Tile>& tiles, float tileSize);

	void RenderMap(const glm::mat4& model, const glm::mat4& view, const glm::mat4& perspective);
};
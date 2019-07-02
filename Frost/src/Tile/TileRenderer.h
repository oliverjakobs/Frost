#pragma once

#include "Tile.h"

#include "Core/Graphics.h"

class TileRenderer
{
private:
	VAO m_vao;

	uint m_instanceCount;

	unique_ptr<TextureAtlas> texture;
public:
	TileRenderer(TextureAtlas* textureAtlas, const std::vector<Tile>& tiles, float tileSize);

	void RenderMap();
	void RenderMap(const std::vector<Tile>& tiles, Image* image);
};
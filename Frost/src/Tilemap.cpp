#include "Tilemap.h"

TileMap::TileMap(const std::string& image, int width, int height, float tileSize, const std::vector<int>& map)
{
	m_width = width;
	m_height = height;

	m_tileSize = tileSize;

	m_image = new Image(image, m_tileSize, m_tileSize);

	for (unsigned int i = 0; i < m_height; i++)
	{
		for (unsigned int j = 0; j < m_width; j++)
		{
			Tile tile;
			tile.position = glm::vec2(j, m_height - (i + 1)) * m_tileSize;
			tile.id = map.at(i * m_width + j);

			m_tiles.push_back(tile);
		}
	}
}

TileMap::~TileMap()
{
	if (m_image != nullptr)
		delete m_image;

	m_tiles.clear();
}

void TileMap::onUpdate()
{
	
}

void TileMap::onRender(Shader* shader) const
{
	for (auto& tile : m_tiles)
	{
		if (tile.id > 0)
			m_image->render(tile.position, shader);
	}
}

void TileMap::onRenderDebug() const
{
	
}
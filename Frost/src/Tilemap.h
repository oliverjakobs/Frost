#pragma once

#include "Scrapbook\Scrapbook.h"

#include <vector>

using namespace sb;

struct Tile
{
	unsigned int id;
	glm::vec2 position;
};

class TileMap
{
private:
	Image* m_image;

	int m_width;
	int m_height;

	float m_tileSize;

	std::vector<Tile> m_tiles;
	
	bool m_changed;
public:
	TileMap(const std::string& image, int width, int height, float tileSize, const std::vector<int>& map);
	~TileMap();

	void onUpdate();
	void onRender(Shader* shader) const;
	void onRenderDebug() const;
};

#pragma once

#include "Scrapbook\Scrapbook.h"
#include "Scrapbook\Framebuffer.h"

#include <vector>

using namespace sb;

enum TileType
{
	Empty = 0,
	Solid = 1,
	Platform = 2,
	SlopeLeft = 3,
	SlopeRight = 4,
	CeilLeft = 5,
	CeilRight = 6
};

struct Tile
{
	unsigned int id;
	glm::vec2 position;
	TileType type;
};

class Body;

class TileMap
{
private:
	Image* m_image;
	FrameBuffer* m_frameBuffer;

	int m_width;
	int m_height;

	float m_tileSize;

	std::vector<Tile> m_tiles;
	std::vector<Body*> m_bodies;

	bool m_changed;
public:
	TileMap(const std::string& image, const std::string& map);
	~TileMap();

	Body* createBody(float x, float y, float w, float h);

	void onChange();
	void onUpdate();
	void onRender();
	void onRenderDebug() const;

	float getTileSize() const;
	glm::vec2 getDimension() const;

	glm::ivec2 getTilePos(float x, float y) const;
	glm::ivec2 getTilePos(const glm::vec2& pos) const;

	std::vector<const Tile*> getAdjacentTiles(float x, float y, float w, float h);
	std::vector<const Tile*> getAdjacentTiles(const glm::vec2& pos, const glm::vec2& size);

	void changeTile(const glm::vec2& pos, unsigned int id, TileType type);
	void changeTile(float x, float y, unsigned int id, TileType type);

	const Tile* getTile(float x, float y) const;
	const Tile* getTile(const glm::vec2& pos) const;
	const Tile* getTileM(int x, int y) const;
	const Tile* getTileM(const glm::ivec2& pos) const;
};

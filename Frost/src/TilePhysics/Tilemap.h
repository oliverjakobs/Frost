#pragma once

#include "Body.h"

enum TileType
{
	Empty = 0,
	Solid = 1,
	Platform = 2,
	SlopeLeft = 3,
	SlopeRight = 4,
	// not implemented yet
	CeilLeft = 5,
	CeilRight = 6
};

struct Tile
{
	unsigned int id;
	glm::vec2 position;
	TileType type;
};

class TileMap
{
private:
	Image* m_image;					// tileset texture
	FrameBuffer* m_frameBuffer;		// render map to fb and only update it when something changed to increase framerate

	int m_width;
	int m_height;

	float m_tileSize;

	glm::vec2 m_gravity;

	std::vector<Tile> m_tiles;
	std::vector<Body*> m_bodies;

	bool m_changed;					// has the map changed; need to update the framebuffer
public:
	TileMap(const std::string& image, const std::string& map, const glm::vec2& gravity = glm::vec2(0.0f, -980));
	~TileMap();

	// only way to create bodies
	Body* createBody(float x, float y, float hWidth, float hHeight, BodyType type);

	// if a tile is changed re-render to the framebuffer
	void onChange();
	// loop functions
	void onUpdate();
	void onRender();
	void onRenderDebug() const;

	float getTileSize() const;
	glm::vec2 getDimension() const;

	// calculate map coords from world coords
	glm::ivec2 getTilePos(float x, float y) const;
	glm::ivec2 getTilePos(const glm::vec2& pos) const;

	// get all tiles in an area (in world coords)
	std::vector<const Tile*> getAdjacentTiles(float x, float y, float w, float h);
	std::vector<const Tile*> getAdjacentTiles(const glm::vec2& pos, const glm::vec2& size);

	// change a tiles id and type
	void changeTile(const glm::vec2& pos, unsigned int id, TileType type);
	void changeTile(float x, float y, unsigned int id, TileType type);

	// get a tile from world coords
	const Tile* getTile(float x, float y) const;
	const Tile* getTile(const glm::vec2& pos) const;
	// get a tile from map coords
	const Tile* getTileM(int x, int y) const;
	const Tile* getTileM(const glm::ivec2& pos) const;

	glm::vec2 getGravity() const;

	// get bodies
	std::vector<Body*> getBodies() const;
	std::vector<Body*> getBodiesT(BodyType type) const;
};

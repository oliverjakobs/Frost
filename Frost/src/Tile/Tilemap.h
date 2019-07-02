#pragma once

#include "Body.h"

#include "TileRenderer.h"

class TileMap
{
private:
	unique_ptr<TileRenderer> m_renderer;

	// map settings
	int m_width;
	int m_height;

	float m_tileSize;

	glm::vec2 m_gravity;

	// stores the time the simulation takes for debug purposes
	float m_simTime;

	// 
	std::vector<Tile> m_tiles;
	std::vector<Line> m_edges;

	std::vector<unique_ptr<Body>> m_bodies;

public:
	TileMap(const std::string& map, const glm::vec2& gravity = glm::vec2(0.0f, -980));
	~TileMap();

	// only way to create bodies
	Body* createBody(float x, float y, float hWidth, float hHeight, BodyType type);
	Body* createBody(const glm::vec2& pos, const glm::vec2& halfDim, BodyType type);
	void destroyBody(Body* body);

	// if a tile is changed re-render to the framebuffer
	void updateEdges();

	// loop functions
	void onUpdate();
	void onRender() const;
	void onRenderDebug() const;

	int getWidth() const;
	int getHeight() const;
	float getTileSize() const;
	glm::vec2 getDimension() const;
	Rect getConstraint() const;

	unsigned int getIndexF(float x, float y) const;
	unsigned int getIndexF(const glm::vec2& pos) const;
	unsigned int getIndex(unsigned int x, unsigned int y) const;
	unsigned int getIndex(const glm::ivec2& pos) const;

	Tile* at(unsigned int index);
	const Tile* at(unsigned int index) const;
	Tile* operator[](unsigned int index);
	const Tile* operator[](unsigned int index) const;

	// calculate map coords from world coords
	glm::ivec2 getTilePos(float x, float y) const;
	glm::ivec2 getTilePos(const glm::vec2& pos) const;

	// get all tiles in an area (in world coords)
	std::vector<const Tile*> getAdjacentTiles(float x, float y, float w, float h);
	std::vector<const Tile*> getAdjacentTiles(const glm::vec2& pos, const glm::vec2& size);
	
	glm::vec2 getGravity() const;
	float getSimulationTime() const;

	std::vector<Tile> getTiles() const;
	std::vector<Line> getEdges() const;

	// get bodies
	std::vector<std::unique_ptr<Body>> const& getBodies() const;
	std::vector<Body*> getBodiesT(BodyType type) const;

	std::vector<Body*> getOtherBodies(Body* body) const;
};

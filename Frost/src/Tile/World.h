#pragma once

#include <string>
#include "TileRenderer.h"
#include "Body.h"

namespace tile
{
	class World
	{
	private:
		std::unique_ptr<TileMap> m_map;
		std::unique_ptr<TileRenderer> m_renderer;

		std::vector<std::shared_ptr<Body>> m_bodies;

		glm::vec2 m_gravity;

		float m_simTime;
	public:
		World(const std::string& map, const glm::vec2& gravity = glm::vec2(0.0f, -980));
		~World();

		// only way to create bodies
		std::shared_ptr<Body> CreateBody(float x, float y, float hWidth, float hHeight, BodyType type);
		std::shared_ptr<Body> CreateBody(const glm::vec2& pos, const glm::vec2& halfDim, BodyType type);
		void DestroyBody(std::shared_ptr<Body> body);

		void Tick(float deltaTime);
		void Render(const glm::vec3& offset, const glm::mat4& viewProjection) const;

		// get all tiles in an area (in world coords)
		std::vector<Tile> GetAdjacentTiles(float x, float y, float w, float h);
		std::vector<Tile> GetAdjacentTiles(const glm::vec2& pos, const glm::vec2& size);

		// get bodies
		std::vector<std::shared_ptr<Body>> GetBodies() const;
		std::vector<std::shared_ptr<Body>> GetBodiesT(BodyType type) const;
		std::vector<std::shared_ptr<Body>> GetOtherBodies(const Body* b) const;

		bool CheckSlope(const glm::vec2& position, TileType slope) const;

		float GetTileSize() const { return m_map->GetTileSize(); }
		const glm::vec2 GetGravity() const { return m_gravity; }
	};
}
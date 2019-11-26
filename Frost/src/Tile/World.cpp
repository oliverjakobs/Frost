#include "World.h"

namespace tile
{
	World::World(const std::string& map, const glm::vec2& gravity)
		: m_gravity(gravity), m_simTime(0.0f)
	{
		// need to be loaded from file
		std::vector<TileID> tileIDs =
		{
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
			2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
			2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
			2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
			2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
			2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
		};

		TypeMap typeMap =
		{
			{ 1, TileType::TILE_SOLID },
			{ 2, TileType::TILE_SLOPE_LEFT },
			{ 3, TileType::TILE_SLOPE_RIGHT },
			{ 4, TileType::TILE_PLATFORM }
		};

		int mapWidth = 64;
		int mapHeight = 25;
		float tileSize = 32.0f;

		int texRows = 1;
		int texColumns = 3;

		// load map
		m_map = std::make_unique<TileMap>(tileIDs, mapWidth, mapHeight, tileSize, typeMap);
		m_renderer = std::make_unique<TileRenderer>(*m_map, std::make_shared<ignis::Texture>("res/textures/tiles.png", texRows, texColumns));
	}

	World::~World()
	{
	}

	std::shared_ptr<Body> World::CreateBody(float x, float y, float hWidth, float hHeight, BodyType type)
	{
		auto body = std::shared_ptr<Body>(new Body(this, glm::vec2(x, y), glm::vec2(hWidth, hHeight), type));

		m_bodies.push_back(body);

		return body;
	}

	std::shared_ptr<Body> World::CreateBody(const glm::vec2& pos, const glm::vec2& halfDim, BodyType type)
	{
		auto body = std::shared_ptr<Body>(new Body(this, pos, halfDim, type));

		m_bodies.push_back(body);

		return body;
	}

	void World::DestroyBody(std::shared_ptr<Body> body)
	{
		auto it = std::find_if(m_bodies.begin(), m_bodies.end(), [&](auto& e) { return e == body; });

		if (it != m_bodies.end())
		{
			m_bodies.erase(it);
		}
	}

	void World::Tick(float deltaTime)
	{
		// measure time for simulation
		float start = 0;

		for (auto& body : m_bodies)
		{
			body->Tick(deltaTime);
		}

		float end = 0;

		m_simTime = end - start;
	}

	void World::Render(const glm::vec3& offset, const glm::mat4& viewProjection) const
	{
		m_renderer->RenderMap(offset, viewProjection);
	}

	std::vector<Tile> World::GetAdjacentTiles(float x, float y, float w, float h)
	{
		std::vector<Tile> tiles;

		glm::ivec2 start = glm::ivec2(static_cast<int>(std::floor(x / GetTileSize())), static_cast<int>(std::floor(y / GetTileSize())));
		glm::ivec2 end = glm::ivec2(static_cast<int>(std::floor((x + w) / GetTileSize())), static_cast<int>(std::floor((y + h) / GetTileSize())));

		for (int i = start.x; i <= end.x; i++)
		{
			for (int j = start.y; j <= end.y; j++)
			{
				if (i < 0 || i >= m_map->GetWidth())
					continue;

				int index = (m_map->GetHeight() - j - 1) * m_map->GetWidth() + i;

				if (index >= 0 && index < m_map->GetTiles().size())
					tiles.push_back(m_map->at(index));
			}
		}

		return tiles;
	}

	std::vector<Tile> World::GetAdjacentTiles(const glm::vec2& pos, const glm::vec2& size)
	{
		return GetAdjacentTiles(pos.x, pos.y, size.x, size.y);
	}

	std::vector<std::shared_ptr<Body>> World::GetBodies() const
	{
		return m_bodies;
	}

	std::vector<std::shared_ptr<Body>> World::GetBodiesT(BodyType type) const
	{
		std::vector<std::shared_ptr<Body>> typed;

		for (auto& b : m_bodies)
		{
			if (b->GetType() == type)
				typed.push_back(b);
		}

		return typed;
	}

	std::vector<std::shared_ptr<Body>> World::GetOtherBodies(const Body* body) const
	{
		std::vector<std::shared_ptr<Body>> others;

		for (auto& b : m_bodies)
		{
			if (b.get() != body)
				others.push_back(b);
		}

		return others;
	}

	bool World::CheckSlope(const glm::vec2& position, TileType slope) const
	{
		size_t index = m_map->GetIndex(position);

		if (index < 0 || index >= m_map->GetTiles().size())
			return false;

		Tile& tile = m_map->at(index);

		return tile.Type == slope;
	}
}

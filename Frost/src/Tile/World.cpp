#include "World.h"

namespace tile
{
	World::World(const std::string& map, const glm::vec2& gravity)
		: m_gravity(gravity)
	{
	}

	World::~World()
	{
	}

	std::shared_ptr<Body> World::CreateBody(float x, float y, float hWidth, float hHeight, BodyType type)
	{
		auto body = std::make_shared<Body>(std::make_shared<World>(this), glm::vec2(x, y), glm::vec2(hWidth, hHeight), type);

		m_bodies.push_back(body);

		return body;
	}

	std::shared_ptr<Body> World::CreateBody(const glm::vec2& pos, const glm::vec2& halfDim, BodyType type)
	{
		auto body = std::make_shared<Body>(std::make_shared<World>(this), pos, halfDim, type);

		m_bodies.push_back(body);

		return body;
	}

	void World::DestroyBody(std::shared_ptr<Body> body)
	{
		auto it = std::find_if(m_bodies.begin(), m_bodies.end(), [&](auto& e) { return e.get() == body; });

		if (it != m_bodies.end())
		{
			m_bodies.erase(it);
		}
	}

	void World::Update(float deltaTime)
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

	void World::Render() const
	{
	}

	std::vector<const Tile&> World::GetAdjacentTiles(float x, float y, float w, float h)
	{
		return std::vector<const Tile&>();
	}

	std::vector<const Tile&> World::GetAdjacentTiles(const glm::vec2& pos, const glm::vec2& size)
	{
		return std::vector<const Tile&>();
	}

	std::vector<std::shared_ptr<Body>> World::GetBodies() const
	{
		return std::vector<std::shared_ptr<Body>>();
	}

	std::vector<std::shared_ptr<Body>> World::GetBodiesT(BodyType type) const
	{
		return std::vector<std::shared_ptr<Body>>();
	}

	std::vector<std::shared_ptr<Body>> World::GetOtherBodies(const Body& b) const
	{
		return std::vector<std::shared_ptr<Body>>();
	}

	bool World::CheckSlope(const glm::vec2& position, TileType slope) const
	{
		Tile& tile = m_map->at(m_map->GetIndex(position));

		return tile.Type == slope;
	}
}

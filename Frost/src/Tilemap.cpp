#include "Tilemap.h"

#include "Scrapbook\Util\stringutils.h"
#include "Scrapbook\Util\fileutils.h"
#include "Scrapbook\Util\utils.h"
#include "Scrapbook\ResourceManager.h"

#include "Body.h"

TileMap::TileMap(const std::string& image, const std::string& map)
{
	auto data = cutStringBefore("[Map]", readFile(map.c_str()));

	auto config = cutString(",", getValue("[Config]", data[0]));
	auto tiles = cutString(",", getValue("[Map]", data[1]));

	m_width = std::stoi(config[0]);
	m_height = std::stoi(config[1]);

	m_tileSize = std::stof(config[2]);

	m_image = new Image(image, m_tileSize, m_tileSize);

	m_frameBuffer = new FrameBuffer(m_width * m_tileSize, m_height * m_tileSize);

	for (unsigned int i = 0; i < m_height; i++)
	{
		for (unsigned int j = 0; j < m_width; j++)
		{
			Tile tile;
			tile.position = glm::vec2(j, m_height - (i + 1)) * m_tileSize;
			tile.id = std::stoi(tiles.at(i * m_width + j));

			if (tile.id == 0)
				tile.type = Empty;
			else
				tile.type = Solid;

			m_tiles.push_back(tile);
		}
	}

	m_changed = true;
}

TileMap::~TileMap()
{
	if (m_image != nullptr)
		delete m_image;

	m_tiles.clear();
}

Body* TileMap::createBody(float x, float y, float w, float h)
{
	Body* body = new Body(this, x, y, w, h);

	m_bodies.push_back(body);

	return body;
}

void TileMap::onChange()
{
	m_frameBuffer->bind();

	for (auto& tile : m_tiles)
	{
		if (tile.id > 0)
			m_image->render(tile.position, ResourceManager::GetShader("shader"));
	}

	m_frameBuffer->unbind();

	m_changed = false;
}

void TileMap::onUpdate()
{
	for (auto& body : m_bodies)
	{
		body->onUpdate();
	}
}

void TileMap::onRender()
{
	if (m_changed)
		onChange();

	Shader* shader = ResourceManager::GetShader("shader");

	shader->use();
	shader->setUniformMat4("projection", glm::mat4(1.0f));
	shader->setUniformMat4("view", glm::mat4(1.0f));
	shader->setUniformMat4("model", glm::mat4(1.0f));
	
	m_frameBuffer->render();
}

void TileMap::onRenderDebug() const
{
	for (auto& tile : m_tiles)
	{
		if (tile.type == Solid)
			Renderer::DrawRect(tile.position.x, tile.position.y, m_tileSize, m_tileSize, RED);
		else if (tile.type == Platform)
			Renderer::DrawRect(tile.position.x, tile.position.y, m_tileSize, m_tileSize, BLUE);
		else if (tile.type == SlopeLeft)
			Renderer::DrawPolygon({ tile.position, tile.position + glm::vec2(m_tileSize, 0.0f),  tile.position + glm::vec2(0.0f, m_tileSize) }, MAGENTA);
		else if (tile.type == SlopeRight)
			Renderer::DrawPolygon({ tile.position, tile.position + glm::vec2(m_tileSize, 0.0f),  tile.position + glm::vec2(m_tileSize) }, MAGENTA);
	}
}

float TileMap::getTileSize() const
{
	return m_tileSize;
}

glm::vec2 TileMap::getDimension() const
{
	return glm::vec2(m_width, m_height);
}

glm::ivec2 TileMap::getTilePos(float x, float y) const
{
	return glm::ivec2(static_cast<int>(std::floor(x / m_tileSize)), static_cast<int>(std::floor(y / m_tileSize)));
}

glm::ivec2 TileMap::getTilePos(const glm::vec2 & pos) const
{
	return getTilePos(pos.x, pos.y);
}

std::vector<const Tile*> TileMap::getAdjacentTiles(float x, float y, float w, float h)
{
	std::vector<const Tile*> tiles;

	glm::ivec2 start = getTilePos(x, y);
	glm::ivec2 end = getTilePos(x + w, y + h);

	for (int i = start.x; i <= end.x; i++)
	{
		for (int j = start.y; j <= end.y; j++)
		{
			const Tile* t = getTileM(i, j);

			if (t != nullptr)
				tiles.push_back(t);
		}
	}

	return tiles;
}

std::vector<const Tile*> TileMap::getAdjacentTiles(const glm::vec2& pos, const glm::vec2& size)
{
	return getAdjacentTiles(pos.x, pos.y, size.x, size.y);
}

void TileMap::changeTile(const glm::vec2& pos, unsigned int id, TileType type)
{
	changeTile(pos.x, pos.y, id, type);
}

void TileMap::changeTile(float x, float y, unsigned int id, TileType type)
{
	glm::ivec2 pos = getTilePos(x, y);

	if (pos.x < 0 || pos.x >= m_width || pos.y < 0 || pos.y >= m_height)
		return;

	Tile* tile = &m_tiles.at((m_height - pos.y - 1) * m_width + pos.x);

	if (!(tile->id == id || tile->type == type))
	{
		tile->id = id;
		tile->type = type;
		m_changed = true;
	}
}

const Tile* TileMap::getTile(float x, float y) const
{
	return getTileM(getTilePos(x, y));
}

const Tile* TileMap::getTile(const glm::vec2& pos) const
{
	return getTile(pos.x, pos.y);
}

const Tile* TileMap::getTileM(int x, int y) const
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height)
		return nullptr;

	return &m_tiles.at((m_height - y - 1) * m_width + x);
}

const Tile* TileMap::getTileM(const glm::ivec2& pos) const
{
	return getTileM(pos.x, pos.y);
}
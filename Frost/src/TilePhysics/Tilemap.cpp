#include "Tilemap.h"

#include "String/StringUtils.h"
#include "File/FileReader.h"

#include "Script/JSONParser.h"

TileMap::TileMap(const std::string& map, const glm::vec2& gravity)
	: m_gravity(gravity)
{
	json root = jsonParseFile(map);

	m_width = jsonToInt(root, "width");
	m_height = jsonToInt(root, "height");

	// tileset
	DEBUG_ASSERT(root.find("tileset") != root.end(), "{0} missing tileset", map);

	json tileset = root.at("tileset");

	m_image = ResourceManager::GetImage(jsonToString(tileset, "name"));
	m_tileSize = jsonToFloat(tileset, "size");

	// load map
	json tiles = root.at("layer");

	m_frameBuffer = unique_ptr<FrameBuffer>(new FrameBuffer((int)std::ceilf(m_width * m_tileSize), (int)std::ceilf(m_height * m_tileSize)));

	// nested loop iterating over the width and height of the map
	for (int i = 0; i < m_height; i++)
	{
		for (int j = 0; j < m_width; j++)
		{
			Tile tile;
			tile.position = glm::vec2(j, m_height - (i + 1)) * m_tileSize;
			tile.id = tiles.at(i * m_width + j);

			// TODO: parse in type
			if (tile.id == 1)
				tile.type = Solid;
			else if (tile.id == 2)
				tile.type = SlopeLeft;
			else if (tile.id == 3)
				tile.type = SlopeRight;
			else if (tile.id == 4)
				tile.type = Platform;
			else
				tile.type = Empty;

			m_tiles.push_back(tile);
		}
	}

	// set to true for the first render
	m_changed = true;
}

TileMap::~TileMap()
{

}

Body* TileMap::createBody(float x, float y, float hWidth, float hHeight, BodyType type)
{
	Body* body = new Body(this, glm::vec2(x, y), glm::vec2(hWidth, hHeight), type);

	m_bodies.push_back(unique_ptr<Body>(body));

	return body;
}

Body* TileMap::createBody(const glm::vec2& pos, const glm::vec2& halfDim, BodyType type)
{
	Body* body = new Body(this, pos, halfDim, type);

	m_bodies.push_back(unique_ptr<Body>(body));

	return body;
}

void TileMap::destroyBody(Body* body)
{
	auto it = std::find_if(m_bodies.begin(), m_bodies.end(), [&](auto& e) { return e.get() == body; });

	if (it != m_bodies.end())
	{
		m_bodies.erase(it);
	}
}

void TileMap::updateFramebuffer() const
{
	m_frameBuffer->bind();

	glClear(GL_COLOR_BUFFER_BIT);

	for (auto& tile : m_tiles)
	{
		m_image->renderF(tile.position, tile.id, m_frameBuffer->getView(), "shader");
	}

	m_frameBuffer->unbind();
}

void TileMap::updateEdges()
{
	struct EdgeData
	{
		int edgeID[4];
		bool edgeExist[4];
	};

	unsigned int northIndex = 0;
	unsigned int southIndex = 1;
	unsigned int eastIndex = 2;
	unsigned int westIndex = 3;

	m_edges.clear();
	m_edges.push_back(Line(0.0f, 0.0f, m_width * m_tileSize, 0.0f));
	m_edges.push_back(Line(m_width * m_tileSize, 0.0f, m_width * m_tileSize, m_height * m_tileSize));
	m_edges.push_back(Line(m_width * m_tileSize, m_height * m_tileSize, 0.0f, m_height * m_tileSize));
	m_edges.push_back(Line(0.0f, m_height * m_tileSize, 0.0f, 0.0f));

	std::vector<EdgeData> edgeData(m_tiles.size());

	// iterate through all tiles from top left to bottom right
	for (int x = 1; x < m_width - 1; x++)
		for (int y = 1; y < m_height - 1; y++)
		{
			// some convenient indices
			int i = y * m_width + x;		// this
			int n = (y - 1) * m_width + x;	// northern neighbour
			int s = (y + 1) * m_width + x;	// southern neighbour
			int w = y * m_width + (x - 1);	// western neighbour
			int e = y * m_width + (x + 1);	// eastern neighbour

			// if this tile is solid, check if it needs edges
			if (m_tiles[i].type == Solid)
			{
				// if there is no western neighbour, it needs a western edge
				if (m_tiles[w].type != Solid)
				{
					// either extend it from its northern neighbour (if they have one) or start a new one
					if (edgeData[n].edgeExist[westIndex])
					{
						// northern neighbour has a western edge, so extend it downwards
						m_edges[edgeData[n].edgeID[westIndex]].end.y += m_tileSize;
						edgeData[i].edgeID[westIndex] = edgeData[n].edgeID[westIndex];
						edgeData[i].edgeExist[westIndex] = true;
					}
					else
					{
						// northern neighbour does not have a western edge, so create one and add it to the polygon pool
						int edgeID = m_edges.size();
						m_edges.push_back(Line(x * m_tileSize, y * m_tileSize, x * m_tileSize, y * m_tileSize + m_tileSize));

						// update tile information with edge information
						edgeData[i].edgeID[westIndex] = edgeID;
						edgeData[i].edgeExist[westIndex] = true;
					}
				}

				// if there is no eastern neighbour, it needs a eastern edge
				if (m_tiles[e].type != Solid)
				{
					// either extend it from its northern neighbour (if they have one) or start a new one
					if (edgeData[n].edgeExist[eastIndex])
					{
						// northern neighbour has a eastern edge, so extend it downwards
						m_edges[edgeData[n].edgeID[eastIndex]].end.y += m_tileSize;
						edgeData[i].edgeID[eastIndex] = edgeData[n].edgeID[eastIndex];
						edgeData[i].edgeExist[eastIndex] = true;
					}
					else
					{
						// northern neighbour does not have a eastern edge, so create one and add it to the polygon pool
						int edgeID = m_edges.size();
						m_edges.push_back(Line((x + 1) * m_tileSize, y * m_tileSize, (x + 1) * m_tileSize, y * m_tileSize + m_tileSize));

						// update tile information with edge information
						edgeData[i].edgeID[eastIndex] = edgeID;
						edgeData[i].edgeExist[eastIndex] = true;
					}
				}

				// if there is no northern neighbour, it needs a northern edge
				if (m_tiles[n].type != Solid)
				{
					// either extend it from its western neighbour (if they have one) or start a new one
					if (edgeData[w].edgeExist[northIndex])
					{
						// western neighbour has a northern edge, so extend it eastwards
						m_edges[edgeData[w].edgeID[northIndex]].end.x += m_tileSize;
						edgeData[i].edgeID[northIndex] = edgeData[w].edgeID[northIndex];
						edgeData[i].edgeExist[northIndex] = true;
					}
					else
					{
						// western neighbour does not have a northern edge, so create one and add it to the polygon pool
						int edgeID = m_edges.size();
						m_edges.push_back(Line(x * m_tileSize, y * m_tileSize, x * m_tileSize + m_tileSize, y * m_tileSize));

						// update tile information with edge information
						edgeData[i].edgeID[northIndex] = edgeID;
						edgeData[i].edgeExist[northIndex] = true;
					}
				}

				// if there is no southern neighbour, it needs a southern edge
				if (m_tiles[s].type != Solid)
				{
					// either extend it from its western neighbour (if they have one) or start a new one
					if (edgeData[w].edgeExist[southIndex])
					{
						// western neighbour has a southern edge, so extend it eastwards
						m_edges[edgeData[w].edgeID[southIndex]].end.x += m_tileSize;
						edgeData[i].edgeID[southIndex] = edgeData[w].edgeID[southIndex];
						edgeData[i].edgeExist[southIndex] = true;
					}
					else
					{
						// western neighbour does not have a southern edge, so create one and add it to the polygon pool
						int edgeID = m_edges.size();
						m_edges.push_back(Line(x * m_tileSize, (y + 1) * m_tileSize, x * m_tileSize + m_tileSize, (y + 1) * m_tileSize));

						// update tile information with edge information
						edgeData[i].edgeID[southIndex] = edgeID;
						edgeData[i].edgeExist[southIndex] = true;
					}
				}
			}
		}
}

void TileMap::onUpdate()
{
	// measure time for simulation
	float start = Timer::GetTimeMS();

	for (auto& body : m_bodies)
	{
		body->onUpdate();
	}

	float end = Timer::GetTimeMS();

	m_simTime = end - start;

	if (m_changed)
	{
		updateEdges();
		m_renderToFB = true;
		m_changed = false;
	}
	else
	{
		m_renderToFB = false;
	}
}

void TileMap::onRender() const
{
	// if the map changed render to the framebuffer first
	if (m_renderToFB)
		updateFramebuffer();
	
	m_frameBuffer->render("shader");
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

	for (auto& body : m_bodies)
	{
		body->onRender();
	}
}

int TileMap::getWidth() const
{
	return m_width;
}

int TileMap::getHeight() const
{
	return m_height;
}

float TileMap::getTileSize() const
{
	return m_tileSize;
}

glm::vec2 TileMap::getDimension() const
{
	return glm::vec2(m_width, m_height);
}

Rect TileMap::getConstraint() const
{
	return  Rect(glm::vec2(), getDimension() * m_tileSize);
}

unsigned int TileMap::getIndexF(float x, float y) const
{
	return getIndex(getTilePos(glm::vec2(x, y)));
}

unsigned int TileMap::getIndexF(const glm::vec2& pos) const
{
	return getIndex(getTilePos(pos));
}

unsigned int TileMap::getIndex(unsigned int x, unsigned int y) const
{
	return (m_height - y - 1) * m_width + x;
}

unsigned int TileMap::getIndex(const glm::ivec2& pos) const
{
	return getIndex(pos.x, pos.y);
}

Tile* TileMap::at(unsigned int index)
{
	if (index >= m_tiles.size())
		return nullptr;

	return &m_tiles.at(index);
}

const Tile* TileMap::at(unsigned int index) const
{
	return &m_tiles.at(index);
}

Tile* TileMap::operator[](unsigned int index)
{
	return &m_tiles[index];
}

const Tile* TileMap::operator[](unsigned int index) const
{
	return &m_tiles[index];
}

glm::ivec2 TileMap::getTilePos(float x, float y) const
{
	return glm::ivec2(static_cast<int>(std::floor(x / m_tileSize)), static_cast<int>(std::floor(y / m_tileSize)));
}

glm::ivec2 TileMap::getTilePos(const glm::vec2& pos) const
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
			if (at(getIndex(i, j)) != nullptr)
				tiles.push_back(at(getIndex(i, j)));
		}
	}

	return tiles;
}

std::vector<const Tile*> TileMap::getAdjacentTiles(const glm::vec2& pos, const glm::vec2& size)
{
	return getAdjacentTiles(pos.x, pos.y, size.x, size.y);
}

glm::vec2 TileMap::getGravity() const
{
	return m_gravity;
}

float TileMap::getSimulationTime() const
{
	return m_simTime;
}

std::vector<Line> TileMap::getEdges() const
{
	return m_edges;
}

std::vector<std::unique_ptr<Body>> const& TileMap::getBodies() const
{
	return m_bodies;
}

std::vector<Body*> TileMap::getBodiesT(BodyType type) const
{
	std::vector<Body*> typed;

	for (auto& b : m_bodies)
	{
		if (b->getType() == type)
			typed.push_back(b.get());
	}

	return typed;
}

std::vector<Body*> TileMap::getOtherBodies(Body* body) const
{
	std::vector<Body*> typed;

	for (auto& b : m_bodies)
	{
		if (b.get() != body)
			typed.push_back(b.get());
	}

	return typed;
}

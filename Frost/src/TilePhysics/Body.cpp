#include "Body.h"

#include "Tilemap.h"

Body::Body(TileMap* map, float x, float y, float hW, float hH, BodyType type)
	: m_map(map), m_position(glm::vec2(x, y)), m_halfDimension(glm::vec2(hW, hH)), m_type(type)
{
	m_velocity = glm::vec2();
	m_sensorOffset = 2.0f;

	m_collidesBottom = false;
	m_collidesTop = false;
	m_collidesLeft = false;
	m_collidesRight = false;
	m_onSlope = false;
	m_slopeDetected = false;

	m_gravityScale = 1.0f;

	m_drop = false;
}

Body::~Body()
{

}

void Body::onUpdate()
{
	if (m_type == BodyTypeStatic)
		return;

	// ------------------ World Collision ----------------------------------
	// vary gravity scale to keep on slope
	if (m_onSlope && m_velocity.y <= 0.0f)
		m_gravityScale = 100.0f;
	else
		m_gravityScale = 1.0f;

	m_velocity += m_map->getGravity() * m_gravityScale * Timer::GetDeltaTime();

	m_offsetHorizontal = glm::vec2(m_sensorOffset, m_onSlope ? 12.0f : 2.0f);
	m_offsetVertical = glm::vec2(2.0f, m_sensorOffset);

	// check for slopes
	if (m_velocity.x < 0.0f)
		m_slopeDetected = checkSlope(m_position + glm::vec2(-(m_halfDimension.x + m_map->getTileSize() - m_sensorOffset), m_sensorOffset - m_halfDimension.y), SlopeLeft);
	else if (m_velocity.x > 0.0f)
		m_slopeDetected = checkSlope(m_position + glm::vec2(m_halfDimension.x + m_map->getTileSize() - m_sensorOffset, m_sensorOffset - m_halfDimension.y), SlopeRight);
	else
		m_slopeDetected = false;

	glm::vec2 oldPos = m_position;

	// move first in x direction and then in y direction
	moveX(m_velocity.x * Timer::GetDeltaTime());
	moveY(m_velocity.y * Timer::GetDeltaTime());

	m_drop = false;

	// ------------------ Object Collision ---------------------------------
	for (auto& b : m_map->getOtherBodies(this))
	{
		resolveBodyCollision(*b, oldPos);
	}
}

void Body::onRender() const
{
	Renderer::DrawRect(m_position - m_halfDimension, m_halfDimension * 2.0f, m_onSlope ? BLUE : GREEN);

	// showing the sensors (only for dynamic bodies)
	if (m_type == BodyTypeDynamic)
	{
		Renderer::DrawLine(getSensorBottom(m_position, m_offsetVertical), RED);
		Renderer::DrawLine(getSensorTop(m_position, m_offsetVertical), RED);
		Renderer::DrawLine(getSensorLeft(m_position, m_offsetHorizontal), RED);
		Renderer::DrawLine(getSensorRight(m_position, m_offsetHorizontal), RED);

		// show slope sensors
		//Renderer::FillCircle(m_position + glm::vec2(-(m_halfDimension.x + m_map->getTileSize() - m_sensorOffset), m_sensorOffset - m_halfDimension.y), 2.0f, RED);
		//Renderer::FillCircle(m_position + glm::vec2(m_halfDimension.x + m_map->getTileSize() - m_sensorOffset, m_sensorOffset - m_halfDimension.y), 2.0f, RED);
	}

	// center/position of the body 
	Renderer::FillCircle(m_position, 2.0f, BLACK);
}

void Body::setPosition(const glm::vec2& pos) { m_position = pos; }

void Body::setVelocity(const glm::vec2& vel) { m_velocity = vel; }

void Body::drop() { m_drop = true; }

glm::vec2 Body::getPosition() const { return m_position; }

glm::vec2 Body::getVelocity() const { return m_velocity; }

float Body::getWidth() const { return m_halfDimension.x * 2.0f; }

float Body::getHeight() const { return m_halfDimension.y * 2.0f; }

float Body::getX() const { return m_position.x - m_halfDimension.x; }

float Body::getX2() const { return m_position.x + m_halfDimension.x; }

float Body::getY() const { return m_position.y - m_halfDimension.y; }

float Body::getY2() const { return m_position.y + m_halfDimension.y; }

bool Body::collidesBottom() const { return m_collidesBottom; }

bool Body::collidesTop() const { return m_collidesTop; }

bool Body::collidesLeft() const { return m_collidesLeft; }

bool Body::collidesRight() const { return m_collidesRight; }

TileMap* Body::getMap() const { return m_map; }

BodyType Body::getType() const { return m_type; }

void Body::moveX(float x)
{
	m_collidesLeft = false;
	m_collidesRight = false;

	glm::vec2 oldPosition = m_position;

	m_position.x += x;

	float wallX = 0.0f;
	if (m_velocity.x <= 0.0f && checkLeft(m_position, oldPosition, &wallX))
	{
		m_position.x = wallX + m_halfDimension.x;
		m_velocity.x = 0.0f;
		m_collidesLeft = true;
	}

	wallX = 0.0f;
	if (m_velocity.x >= 0.0f && checkRight(m_position, oldPosition, &wallX))
	{
		m_position.x = wallX - m_halfDimension.x;
		m_velocity.x = 0.0f;
		m_collidesRight = true;
	}
}

void Body::moveY(float y)
{
	m_collidesBottom = false;
	m_collidesTop = false;

	glm::vec2 oldPosition = m_position;
	m_position.y += y;

	m_onSlope = false;

	float groundY = 0.0f;
	if (m_velocity.y <= 0.0f && checkBottom(m_position, oldPosition, &groundY))
	{
		m_position.y = groundY + m_halfDimension.y;
		m_velocity.y = 0.0f;
		m_collidesBottom = true;
	}

	groundY = 0.0f;
	if (m_velocity.y >= 0.0f && checkTop(m_position, oldPosition, &groundY))
	{
		m_position.y = groundY - m_halfDimension.y;
		m_velocity.y = 0.0f;
		m_collidesTop = true;
	}
}

bool Body::checkBottom(const glm::vec2& position, const glm::vec2& oldPosition, float* groundY)
{
	Line sensor = getSensorBottom(position, m_offsetVertical);
	Line oldSensor = getSensorBottom(oldPosition, m_offsetVertical);

	auto tiles = m_map->getAdjacentTiles(sensor.start, oldSensor.end - sensor.start + (m_slopeDetected ? glm::vec2(0.0f, m_map->getTileSize() / 2.0f) : glm::vec2()));

	std::sort(tiles.begin(), tiles.end(), [](const Tile* t1, const Tile* t2)
	{
		if (t1->position.y == t2->position.y)
			return t1->type < t2->type;

		return t1->position.y > t2->position.y;
	});

	for (auto& t : tiles)
	{
		if (t->type == Solid && !m_onSlope)
		{
			*groundY = t->position.y + m_map->getTileSize();
			return true;
		}
		else if (t->type == SlopeLeft)
		{
			float x = m_position.x - m_halfDimension.x - t->position.x;

			*groundY = t->position.y + m_map->getTileSize() - x;
			m_onSlope = true;
			return true;
		}
		else if (t->type == SlopeRight)
		{
			float x = m_position.x + m_halfDimension.x - t->position.x;

			*groundY = t->position.y + x;
			m_onSlope = true;
			return true;
		}
		else if (t->type == Platform && !m_drop)
		{
			*groundY = t->position.y + m_map->getTileSize();
			if ((oldPosition.y - m_halfDimension.y) >= *groundY)
				return true;
		}
	}

	return false;
}

bool Body::checkTop(const glm::vec2& position, const glm::vec2& oldPosition, float* groundY)
{
	Line sensor = getSensorTop(position, m_offsetVertical);
	Line oldSensor = getSensorTop(oldPosition, m_offsetVertical);

	auto tiles = m_map->getAdjacentTiles(oldSensor.start, sensor.end - oldSensor.start);

	std::sort(tiles.begin(), tiles.end(), [](const Tile* t1, const Tile* t2)
	{
		return t1->position.y < t2->position.y;
	});

	for (auto& t : tiles)
	{
		if (t->type == Solid)
		{
			*groundY = t->position.y;
			return true;
		}
	}

	return false;
}

bool Body::checkLeft(const glm::vec2& position, const glm::vec2& oldPosition, float* wallX)
{
	Line sensor = getSensorLeft(position, m_offsetHorizontal);
	Line oldSensor = getSensorLeft(oldPosition, m_offsetHorizontal);

	auto tiles = m_map->getAdjacentTiles(sensor.start, oldSensor.end - sensor.start);

	std::sort(tiles.begin(), tiles.end(), [](const Tile* t1, const Tile* t2)
	{
		return t1->position.x > t2->position.x;
	});

	for (auto& t : tiles)
	{
		if (t->type == Solid)
		{
			*wallX = t->position.x + m_map->getTileSize();
			return true;
		}
	}

	return false;
}

bool Body::checkRight(const glm::vec2& position, const glm::vec2& oldPosition, float* wallX)
{
	Line sensor = getSensorRight(position, m_offsetHorizontal);
	Line oldSensor = getSensorRight(oldPosition, m_offsetHorizontal);

	auto tiles = m_map->getAdjacentTiles(oldSensor.start, sensor.end - oldSensor.start);

	std::sort(tiles.begin(), tiles.end(), [](const Tile* t1, const Tile* t2)
	{
		return t1->position.x < t2->position.x;
	});

	for (auto& t : tiles)
	{
		if (t->type == Solid)
		{
			*wallX = t->position.x;
			return true;
		}
	}

	return false;
}

bool Body::checkSlope(const glm::vec2& position, int slope) const
{
	Tile* tile = m_map->at(m_map->getIndex(position));

	if (tile == nullptr)
		return false;

	return tile->type == slope;
}

void Body::resolveBodyCollision(const Body& b, const glm::vec2& oldPos)
{
	glm::vec2 overlap;

	overlap.x = std::min(getX2() - b.getX(), b.getX2() - getX());
	overlap.y = std::min((oldPos.y + m_halfDimension.y) - b.getY(), b.getY2() - (oldPos.y - m_halfDimension.y));

	// horizontal resolve
	if (overlap.x > 0.0f && overlap.y > 0.0f)
	{
		if (m_velocity.x < 0.0f)
		{
			m_position.x += std::max(overlap.x, 0.0f);
			m_velocity.x = 0.0f;
			m_collidesLeft = true;
		}
		else if (m_velocity.x > 0.0f)
		{
			m_position.x -= std::max(overlap.x, 0.0f);
			m_velocity.x = 0.0f;
			m_collidesRight = true;
		}
	}

	overlap.x = std::min((oldPos.x + m_halfDimension.x) - b.getX(), b.getX2() - (oldPos.x - m_halfDimension.x));
	overlap.y = std::min(getY2() - b.getY(), b.getY2() - getY());

	// vertical resolve
	if (overlap.x > 0.0f && overlap.y > 0.0f)
	{
		if (m_velocity.y < 0.0f)
		{
			m_position.y += std::max(overlap.y, 0.0f);
			m_velocity.y = 0.0f;
			m_collidesBottom = true;
		}
		else if (m_velocity.y > 0.0f)
		{
			m_position.y -= std::max(overlap.y, 0.0f);
			m_velocity.y = 0.0f;
			m_collidesTop = true;
		}
	}
}

Line Body::getSensorBottom(const glm::vec2& pos, const glm::vec2& offset) const
{
	return Line(pos.x - m_halfDimension.x + offset.x, pos.y - m_halfDimension.y - offset.y, pos.x + m_halfDimension.x - offset.x, pos.y - m_halfDimension.y - offset.y);
}

Line Body::getSensorTop(const glm::vec2& pos, const glm::vec2& offset) const
{
	return Line(pos.x - m_halfDimension.x + offset.x, pos.y + m_halfDimension.y + offset.y, pos.x + m_halfDimension.x - offset.x, pos.y + m_halfDimension.y + offset.y);
}

Line Body::getSensorLeft(const glm::vec2& pos, const glm::vec2& offset) const
{
	return Line(pos.x - m_halfDimension.x - offset.x, pos.y - m_halfDimension.y + offset.y, pos.x - m_halfDimension.x - offset.x, pos.y + m_halfDimension.y - offset.y);
}

Line Body::getSensorRight(const glm::vec2& pos, const glm::vec2& offset) const
{
	return Line(pos.x + m_halfDimension.x + offset.x, pos.y - m_halfDimension.y + offset.y, pos.x + m_halfDimension.x + offset.x, pos.y + m_halfDimension.y - offset.y);
}
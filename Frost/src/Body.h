#pragma once

#include "aabb.h"
#include "Tilemap.h"

class Body
{
private:
	glm::vec2 m_position;

	AABB m_aabb;
	glm::vec2 m_aabbOffset;

	glm::vec2 m_velocity;

	glm::vec2 m_offsetHorizontal;
	glm::vec2 m_offsetVertical;

	bool m_collidesBottom;
	bool m_collidesTop;
	bool m_collidesLeft;
	bool m_collidesRight;

	bool m_onSlope;
	bool m_slopeDetected;

	float m_gravityScale;
	float m_sensorOffset;

	bool m_drop;

	TileMap* m_map;
	Body(TileMap* map, float x, float y, float w, float h);
	virtual ~Body();

	void onUpdate();
public:
	void onRender() const;

	void setPosition(const glm::vec2& pos);
	void setVelocity(const glm::vec2& vel);

	void drop();

	glm::vec2 getPosition() const;
	glm::vec2 getVelocity() const;

	bool collidesBottom() const;

	TileMap* getMap() const;

	friend TileMap;
private:
	void moveX(float x);
	void moveY(float y);

	bool checkBottom(const glm::vec2& position, const glm::vec2& oldPosition, float* groundY);
	bool checkTop(const glm::vec2& position, const glm::vec2& oldPosition, float* groundY);
	bool checkLeft(const glm::vec2& position, const glm::vec2& oldPosition, float* wallX);
	bool checkRight(const glm::vec2& position, const glm::vec2& oldPosition, float* wallX);
	bool checkSlope(const glm::vec2& position, int slope) const;

	Line getSensorBottom(const glm::vec2& center, const glm::vec2& offset) const;
	Line getSensorTop(const glm::vec2& center, const glm::vec2& offset) const;
	Line getSensorLeft(const glm::vec2& center, const glm::vec2& offset) const;
	Line getSensorRight(const glm::vec2& center, const glm::vec2& offset) const;
};
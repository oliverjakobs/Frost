#pragma once

#include "Maths/Maths.h"
#include "TileConfig.h"

class TileMap;

enum BodyType
{
	BodyTypeStatic,		// does not move by any means 
	BodyTypeDynamic		// can be moved, affected by forces
};

struct BodyDef
{
	float hW;
	float hH;
	BodyType type;
};

class Body
{
private:
	glm::vec2 m_position;
	glm::vec2 m_halfDimension;

	glm::vec2 m_velocity;

	// -----------------------------------
	float m_sensorOffset;			// offset to the edges
	glm::vec2 m_offsetHorizontal;
	glm::vec2 m_offsetVertical;

	// CollisionState
	bool m_collidesBottom;
	bool m_collidesTop;
	bool m_collidesLeft;
	bool m_collidesRight;

	// Slopes
	bool m_onSlope;
	bool m_slopeDetected;

	float m_gravityScale;	// amplifies the magnitude of the gravity
	bool m_drop;			// should the body drop through a platform

	TileMap* m_map;
	BodyType m_type;

	Body(TileMap* map, float x, float y, float hW, float hH, BodyType type);

	// can only be updated by the map
	void onUpdate();
public:
	virtual ~Body();
	// renders different debugging info
	void onRender() const;

	void setPosition(const glm::vec2& pos);
	void setVelocity(const glm::vec2& vel);

	void drop();

	glm::vec2 getPosition() const;
	glm::vec2 getVelocity() const;

	float getWidth() const;
	float getHeight() const;

	// get the edges of the body
	float getX() const;
	float getX2() const;
	float getY() const;
	float getY2() const;

	// get the collision state
	bool collidesBottom() const;
	bool collidesTop() const;
	bool collidesLeft() const;
	bool collidesRight() const;

	TileMap* getMap() const;
	BodyType getType() const;

	// body creation and updating is done by the map
	friend TileMap;
private:
	// move in x and y direction seperatly
	void moveX(float x);
	void moveY(float y);

	// world collision detection
	bool checkBottom(const glm::vec2& position, const glm::vec2& oldPosition, float* groundY);
	bool checkTop(const glm::vec2& position, const glm::vec2& oldPosition, float* groundY);
	bool checkLeft(const glm::vec2& position, const glm::vec2& oldPosition, float* wallX);
	bool checkRight(const glm::vec2& position, const glm::vec2& oldPosition, float* wallX);
	bool checkSlope(const glm::vec2& position, int slope) const;

	// body collision
	void resolveBodyCollision(const Body& b, const glm::vec2& oldPos);

	// get sensors around the position
	Line getSensorBottom(const glm::vec2& position, const glm::vec2& offset) const;
	Line getSensorTop(const glm::vec2& position, const glm::vec2& offset) const;
	Line getSensorLeft(const glm::vec2& position, const glm::vec2& offset) const;
	Line getSensorRight(const glm::vec2& position, const glm::vec2& offset) const;
};
#pragma once

#include <glm/glm.hpp>

struct World;

enum class BodyType
{
	STATIC,
	DYNAMIC
};

struct Body
{
	glm::vec2 position;
	glm::vec2 halfSize;

	glm::vec2 velocity;

	BodyType type;

	// CollisionState
	bool collidesBottom;
	bool collidesTop;
	bool collidesLeft;
	bool collidesRight;

	World* world;
};

void BodyLoad(Body* body, float x, float y, float hw, float hh, BodyType type);

void BodyTick(Body* body, float deltatime);

void BodyResolveCollision(Body* body, const Body* other, const glm::vec2& oldpos);

/* get the edges of the body */
const float BodyGetX(const Body* body);
const float BodyGetX2(const Body* body);
const float BodyGetY(const Body* body);
const float BodyGetY2(const Body* body);
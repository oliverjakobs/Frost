#include "Body.hpp"

#include "World.hpp"

#include "clib/clib.h"

void BodyLoad(Body* body, float x, float y, float hw, float hh, BodyType type)
{
	body->position = glm::vec2(x, y);
	body->halfSize = glm::vec2(hw, hh);
	body->velocity = glm::vec2(0.0f, 0.0f);
	
	body->type = type;
	
	body->world = nullptr;

	body->collidesBottom = false;
	body->collidesTop = false;
	body->collidesLeft = false;
	body->collidesRight = false;
}

void BodyTick(Body* body, float deltatime)
{
	body->velocity += body->world->gravity * deltatime;

	body->collidesBottom = false;
	body->collidesTop = false;
	body->collidesLeft = false;
	body->collidesRight = false;

	body->position += body->velocity * deltatime;
}

void BodyResolveCollision(Body* body, const Body* other, const glm::vec2& oldpos)
{
	float overlap_x = MIN(BodyGetX2(body) - BodyGetX(other), BodyGetX2(other) - BodyGetX(body));
	float overlap_y = MIN((oldpos.y + body->halfSize.y) - BodyGetY(other), BodyGetY2(other) - (oldpos.y - body->halfSize.y));

	// horizontal resolve
	if (overlap_x > 0.0f && overlap_y > 0.0f)
	{
		if (body->velocity.x < 0.0f)
		{
			body->position.x += MAX(overlap_x, 0.0f);
			body->velocity.x = 0.0f;
			body->collidesLeft = true;
		}
		else if (body->velocity.x > 0.0f)
		{
			body->position.x -= MAX(overlap_x, 0.0f);
			body->velocity.x = 0.0f;
			body->collidesRight = true;
		}
	}

	overlap_x = MIN((oldpos.x + body->halfSize.x) - BodyGetX(other), BodyGetX2(other) - (oldpos.x - body->halfSize.x));
	overlap_y = MIN(BodyGetY2(body) - BodyGetY(other), BodyGetY2(other) - BodyGetY(body));

	// vertical resolve
	if (overlap_x > 0.0f && overlap_y > 0.0f)
	{
		if (body->velocity.y < 0.0f)
		{
			body->position.y += MAX(overlap_y, 0.0f);
			body->velocity.y = 0.0f;
			body->collidesBottom = true;
		}
		else if (body->velocity.y > 0.0f)
		{
			body->position.y -= MAX(overlap_y, 0.0f);
			body->velocity.y = 0.0f;
			body->collidesTop = true;
		}
	}
}

const float BodyGetX(const Body* body)
{
	return body->position.x - body->halfSize.x;
}

const float BodyGetX2(const Body* body)
{
	return body->position.x + body->halfSize.x;
}

const float BodyGetY(const Body* body)
{
	return body->position.y - body->halfSize.y;
}

const float BodyGetY2(const Body* body)
{
	return body->position.y + body->halfSize.y;
}

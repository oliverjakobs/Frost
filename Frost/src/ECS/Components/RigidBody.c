#include "RigidBody.h"

void RigidBodyTick(RigidBody* body, vec2 gravity, float deltatime)
{
	body->velocity = vec2_add(body->velocity, vec2_mult(gravity, deltatime));

	body->collides_bottom = 0;
	body->collides_top = 0;
	body->collides_left = 0;
	body->collides_right = 0;

	body->position = vec2_add(body->position, vec2_mult(body->velocity, deltatime));
}

void RigidBodyResolveCollision(RigidBody* body, const RigidBody* other, vec2 oldpos)
{
	float x0 = (body->position.x + body->half_size.x) - (other->position.x - other->half_size.x);
	float x1 = (other->position.x + other->half_size.x) - (body->position.x - body->half_size.x);

	float y0 = (oldpos.y + body->half_size.y) - (other->position.y - other->half_size.y);
	float y1 = (other->position.y + other->half_size.y) - (oldpos.y - body->half_size.y);

	float overlap_x = MIN(x0, x1);
	float overlap_y = MIN(y0, y1);

	// horizontal resolve
	if (overlap_x > 0.0f && overlap_y > 0.0f)
	{
		if (body->velocity.x < 0.0f)
		{
			body->position.x += MAX(overlap_x, 0.0f);
			body->velocity.x = 0.0f;
			body->collides_left = 1;
		}
		else if (body->velocity.x > 0.0f)
		{
			body->position.x -= MAX(overlap_x, 0.0f);
			body->velocity.x = 0.0f;
			body->collides_right = 1;
		}
	}

	x0 = (oldpos.x + body->half_size.x) - (other->position.x - other->half_size.x);
	x1 = (other->position.x + other->half_size.x) - (oldpos.x - body->half_size.x);

	y0 = (body->position.y + body->half_size.y) - (other->position.y - other->half_size.y);
	y1 = (other->position.y + other->half_size.y) - (body->position.y - body->half_size.y);

	overlap_x = MIN(x0, x1);
	overlap_y = MIN(y0, y1);

	// vertical resolve
	if (overlap_x > 0.0f && overlap_y > 0.0f)
	{
		if (body->velocity.y < 0.0f)
		{
			body->position.y += MAX(overlap_y, 0.0f);
			body->velocity.y = 0.0f;
			body->collides_bottom = 1;
		}
		else if (body->velocity.y > 0.0f)
		{
			body->position.y -= MAX(overlap_y, 0.0f);
			body->velocity.y = 0.0f;
			body->collides_top = 1;
		}
	}
}

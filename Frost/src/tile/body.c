#include "body.h"

#include "toolbox/toolbox.h"


// To show a body's sensors on render
#define TILE_SHOW_SENSOR 0
#define TILE_SHOW_SLOPE_SENSOR 0

//
#define TILE_SENSOR_OFFSET 2.0f

// 
#define TILE_SLOPE_GRIP 100.0f

void TileBodyInit(TileBody* body, float x, float y, float h_w, float h_h)
{
	body->position = (vec2){ x, y };
	body->half_dim = (vec2){ h_w, h_h };
	
	body->velocity = vec2_zero();
	body->sensor_offset = TILE_SENSOR_OFFSET;

	body->collides_bottom = 0;
	body->collides_top = 0;
	body->collides_left = 0;
	body->collides_right = 0;
	body->on_slope = 0;
	body->slope_detected = 0;
	body->drop = 0;

	body->gravity_scale = 1.0f;

	body->offset_horizontal = vec2_zero();
	body->offset_vertical = vec2_zero();
}

void TileBodyMoveX(TileBody* body, float x)
{
	body->collides_left = 0;
	body->collides_right = 1;

	vec2 old_position = body->position;
	body->position.x += x;

	float wall_x = 0.0f;
	if (body->velocity.x <= 0.0f && TileBodyCheckLeft(body, body->position, old_position, &wall_x))
	{
		body->position.x = wall_x + body->half_dim.x;
		body->velocity.x = 0.0f;
		body->collides_left = 1;
	}

	wall_x = 0.0f;
	if (body->velocity.x >= 0.0f && TileBodyCheckRight(body, body->position, old_position, &wall_x))
	{
		body->position.x = wall_x - body->half_dim.x;
		body->velocity.x = 0.0f;
		body->collides_right = 1;
	}
}

void TileBodyMoveY(TileBody* body, float y)
{
	body->collides_bottom = 0;
	body->collides_top = 0;

	vec2 old_position = body->position;
	body->position.y += y;

	body->on_slope = 0;

	float ground_y = 0.0f;
	if (body->velocity.y <= 0.0f && TileBodyCheckBottom(body, body->position, old_position, &ground_y))
	{
		body->position.y = ground_y + body->half_dim.y;
		body->velocity.y = 0.0f;
		body->collides_bottom = 1;
	}

	ground_y = 0.0f;
	if (body->velocity.y >= 0.0f && TileBodyCheckTop(body, body->position, old_position, &ground_y))
	{
		body->position.y = ground_y - body->half_dim.y;
		body->velocity.y = 0.0f;
		body->collides_top = 1;
	}
}

void TileBodyResolveMap(TileBody* body, TileMap* map, vec2 gravity, float deltatime)
{
	if (body->on_slope && body->velocity.y <= 0.0f)
	{
		body->velocity.x += gravity.x * body->gravity_scale * TILE_SLOPE_GRIP * deltatime;
		body->velocity.y += gravity.y * body->gravity_scale * TILE_SLOPE_GRIP * deltatime;
	}
	else
	{
		body->velocity.x += gravity.x * body->gravity_scale * deltatime;
		body->velocity.y += gravity.y * body->gravity_scale * deltatime;
	}

	body->offset_horizontal = (vec2){ body->sensor_offset, body->on_slope ? 12.0f : 2.0f };
	body->offset_vertical = (vec2){ 2.0f, body->sensor_offset };

	// check for slopes
	body->slope_detected = 0;
	if (body->velocity.x < 0.0f)
	{
		// far sensors
		vec2 offset = (vec2){ -(body->half_dim.x + map->tile_size - body->sensor_offset), body->sensor_offset - body->half_dim.y };
		body->slope_detected = TileMapCheckType(map, vec2_add(body->position, offset), TILE_SLOPE_LEFT);
		// near sensors
		offset = (vec2){ -(body->half_dim.x + (map->tile_size / 2.0f) - body->sensor_offset), body->sensor_offset - body->half_dim.y };
		body->slope_detected |= TileMapCheckType(map, vec2_add(body->position, offset), TILE_SLOPE_LEFT);
	}
	else if (body->velocity.x > 0.0f)
	{
		// far sensors
		vec2 offset = (vec2){ body->half_dim.x + map->tile_size - body->sensor_offset, body->sensor_offset - body->half_dim.y };
		body->slope_detected = TileMapCheckType(map, vec2_add(body->position, offset), TILE_SLOPE_RIGHT);
		// near sensors
		offset = (vec2){ body->half_dim.x + (map->tile_size / 2.0f) - body->sensor_offset, body->sensor_offset - body->half_dim.y };
		body->slope_detected |= TileMapCheckType(map, vec2_add(body->position, offset), TILE_SLOPE_RIGHT);
	}

	// move first in x direction and then in y direction
	TileBodyMoveX(body, body->velocity.x * deltatime);
	TileBodyMoveY(body, body->velocity.y * deltatime);

	body->drop = 0;
}

void TileBodyResolveBody(TileBody* body, TileBody* other, vec2 old_pos)
{
	float x0 = (body->position.x + body->half_dim.x) - (other->position.x - other->half_dim.x);
	float x1 = (other->position.x + other->half_dim.x) - (body->position.x - body->half_dim.x);

	float y0 = (old_pos.y + body->half_dim.y) - (other->position.y - other->half_dim.y);
	float y1 = (other->position.y + other->half_dim.y) - (old_pos.y - body->half_dim.y);

	float overlap_x = tb_min_f(x0, x1);
	float overlap_y = tb_min_f(y0, y1);

	// horizontal resolve
	if (overlap_x > 0.0f && overlap_y > 0.0f)
	{
		if (body->velocity.x < 0.0f)
		{
			body->position.x += tb_max_f(overlap_x, 0.0f);
			body->velocity.x = 0.0f;
			body->collides_left = 1;
		}
		else if (body->velocity.x > 0.0f)
		{
			body->position.x -= tb_max_f(overlap_x, 0.0f);
			body->velocity.x = 0.0f;
			body->collides_right = 1;
		}
	}

	x0 = (old_pos.x + body->half_dim.x) - (other->position.x - other->half_dim.x);
	x1 = (other->position.x + other->half_dim.x) - (old_pos.x - body->half_dim.x);

	y0 = (body->position.y + body->half_dim.y) - (other->position.y - other->half_dim.y);
	y1 = (other->position.y + other->half_dim.y) - (body->position.y - body->half_dim.y);

	overlap_x = tb_min_f(x0, x1);
	overlap_y = tb_min_f(y0, y1);

	// vertical resolve
	if (overlap_x > 0.0f && overlap_y > 0.0f)
	{
		if (body->velocity.y < 0.0f)
		{
			body->position.y += tb_max_f(overlap_y, 0.0f);
			body->velocity.y = 0.0f;
			body->collides_bottom = 1;
		}
		else if (body->velocity.y > 0.0f)
		{
			body->position.y -= tb_max_f(overlap_y, 0.0f);
			body->velocity.y = 0.0f;
			body->collides_top = 1;
		}
	}
}

float TileBodyGetX(const TileBody* body) { return body->position.x - body->half_dim.x; }
float TileBodyGetY(const TileBody* body) { return body->position.y - body->half_dim.y; }
float TileBodyGetX2(const TileBody* body) { return body->position.x + body->half_dim.x; }
float TileBodyGetY2(const TileBody* body) { return body->position.y + body->half_dim.y; }


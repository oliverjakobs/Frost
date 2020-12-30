#include "body.h"

#include "toolbox/toolbox.h"


// To show a body's sensors on render
#define TILE_SHOW_SENSOR 0
#define TILE_SHOW_SLOPE_SENSOR 0

//
#define TILE_SENSOR_OFFSET 2.0f

// 
#define TILE_SLOPE_GRIP 100.0f

void TileBodyInit(TileBody* body, const TileMap* map, float x, float y, float h_w, float h_h)
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

	body->map = map;
}

void TileBodyMoveX(TileBody* body, float x)
{
	body->collides_left = 0;
	body->collides_right = 1;

	vec2 old_pos = body->position;
	body->position.x += x;

	float wall_x = 0.0f;
	if (body->velocity.x <= 0.0f && TileBodyCheckLeft(body, body->position, old_pos, &wall_x))
	{
		body->position.x = wall_x + body->half_dim.x;
		body->velocity.x = 0.0f;
		body->collides_left = 1;
	}

	wall_x = 0.0f;
	if (body->velocity.x >= 0.0f && TileBodyCheckRight(body, body->position, old_pos, &wall_x))
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

	vec2 old_pos = body->position;
	body->position.y += y;

	body->on_slope = 0;

	float ground_y = 0.0f;
	if (body->velocity.y <= 0.0f && TileBodyCheckBottom(body, body->position, old_pos, &ground_y))
	{
		body->position.y = ground_y + body->half_dim.y;
		body->velocity.y = 0.0f;
		body->collides_bottom = 1;
	}

	ground_y = 0.0f;
	if (body->velocity.y >= 0.0f && TileBodyCheckTop(body, body->position, old_pos, &ground_y))
	{
		body->position.y = ground_y - body->half_dim.y;
		body->velocity.y = 0.0f;
		body->collides_top = 1;
	}
}

static int CmpTilesBottom(const Tile** left, const Tile** right)
{
	if ((*left)->pos.y == (*right)->pos.y)
		return (*left)->type - (*right)->type;

	return (*right)->pos.y - (*left)->pos.y;
}

int TileBodyCheckBottom(TileBody* body, vec2 pos, vec2 old_pos, float* ground_y)
{
	line sensor = TileBodyGetSensor(body, TILE_BOTTOM, pos, body->offset_vertical);
	line old_sensor = TileBodyGetSensor(body, TILE_BOTTOM, old_pos, body->offset_vertical);

	float area_x = old_sensor.start.x;
	float area_y = old_sensor.start.y;
	float area_w = sensor.end.x - area_x;
	float area_h = sensor.end.y - area_y + (body->slope_detected ? body->map->tile_size / 2.0f : 0.0f);

	Tile* tiles[64];
	size_t tile_count = TileMapGetArea(body->map, tiles, 64, area_x, area_y, area_w, area_h);
		
	for (size_t i = 0; i < tile_count; ++i)
	{
		Tile* tile = tiles[i];
		switch (tile->type)
		{
		case TILE_SOLID:
			if (body->on_slope) break;

			*ground_y = tile->pos.y + body->map->tile_size;
			return 1;
		case TILE_SLOPE_LEFT:
			*ground_y = tile->pos.y + body->map->tile_size - (body->position.x - body->half_dim.x - tile->pos.x);
			body->on_slope = 1;
			return 1;
		case TILE_SLOPE_RIGHT:
			*ground_y = tile->pos.y + (body->position.x + body->half_dim.x + tile->pos.x);
			body->on_slope = 1;
			return 1;
		case TILE_PLATFORM:
			if (body->drop) break;

			*ground_y = tile->pos.y + body->map->tile_size;
			if ((old_pos.y - body->half_dim.y) >= *ground_y)
				return 1;
		}
	}

	return 0;
}

int TileBodyCheckTop(TileBody* body, vec2 pos, vec2 old_pos, float* ground_y)
{
	return 0;
}

int TileBodyCheckLeft(TileBody* body, vec2 pos, vec2 old_pos, float* wall_x)
{
	return 0;
}

int TileBodyCheckRight(TileBody* body, vec2 pos, vec2 old_pos, float* wall_y)
{
	return 0;
}

void TileBodyResolveMap(TileBody* body, vec2 gravity, float deltatime)
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

	float tile_size = body->map->tile_size;

	// check for slopes
	body->slope_detected = 0;
	if (body->velocity.x < 0.0f)
	{
		// far sensors
		vec2 offset = (vec2){ -(body->half_dim.x + tile_size - body->sensor_offset), body->sensor_offset - body->half_dim.y };
		body->slope_detected = TileMapCheckType(body->map, vec2_add(body->position, offset), TILE_SLOPE_LEFT);
		// near sensors
		offset = (vec2){ -(body->half_dim.x + (tile_size / 2.0f) - body->sensor_offset), body->sensor_offset - body->half_dim.y };
		body->slope_detected |= TileMapCheckType(body->map, vec2_add(body->position, offset), TILE_SLOPE_LEFT);
	}
	else if (body->velocity.x > 0.0f)
	{
		// far sensors
		vec2 offset = (vec2){ body->half_dim.x + tile_size - body->sensor_offset, body->sensor_offset - body->half_dim.y };
		body->slope_detected = TileMapCheckType(body->map, vec2_add(body->position, offset), TILE_SLOPE_RIGHT);
		// near sensors
		offset = (vec2){ body->half_dim.x + (tile_size / 2.0f) - body->sensor_offset, body->sensor_offset - body->half_dim.y };
		body->slope_detected |= TileMapCheckType(body->map, vec2_add(body->position, offset), TILE_SLOPE_RIGHT);
	}

	// move first in x direction and then in y direction
	TileBodyMoveX(body, body->velocity.x * deltatime);
	TileBodyMoveY(body, body->velocity.y * deltatime);

	body->drop = 0;
}

void TileBodyResolveBody(TileBody* body, const TileBody* other, vec2 old_pos)
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

line TileBodyGetSensor(const TileBody* body, TileDirection dir, vec2 pos, vec2 offset)
{
	line l;
	switch (dir)
	{
	case TILE_LEFT:
		l.start = (vec2){ pos.x - body->half_dim.x - offset.x, pos.y - body->half_dim.y + offset.y };
		l.end = (vec2){ pos.x - body->half_dim.x - offset.x, pos.y + body->half_dim.y - offset.y };
		break;
	case TILE_RIGHT:
		l.start = (vec2){ pos.x + body->half_dim.x + offset.x, pos.y - body->half_dim.y + offset.y };
		l.end = (vec2){ pos.x + body->half_dim.x + offset.x, pos.y + body->half_dim.y - offset.y };
		break;
	case TILE_TOP:
		l.start = (vec2){ pos.x - body->half_dim.x + offset.x, pos.y + body->half_dim.y + offset.y };
		l.end = (vec2){ pos.x + body->half_dim.x - offset.x, pos.y + body->half_dim.y + offset.y };
		break;
	case TILE_BOTTOM:
		l.start = (vec2){ pos.x - body->half_dim.x + offset.x, pos.y - body->half_dim.y - offset.y };
		l.end = (vec2){ pos.x + body->half_dim.x - offset.x, pos.y - body->half_dim.y - offset.y };
		break;
	default:
		l.start = vec2_zero();
		l.end = vec2_zero();
		break;
	}
	return l;
}
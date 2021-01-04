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

	body->collision_state[TILE_LEFT] = 0;
	body->collision_state[TILE_RIGHT] = 0;
	body->collision_state[TILE_TOP] = 0;
	body->collision_state[TILE_BOTTOM] = 0;

	body->on_slope = 0;
	body->slope_detected = 0;
	body->drop = 0;

	body->gravity_scale = 1.0f;

	body->map = map;
}

void TileBodyMoveX(TileBody* body, float x)
{
	vec2 old_pos = body->position;
	body->position.x += x;

	float wall_x = 0.0f;
	if (body->velocity.x <= 0.0f && TileBodyCheckLeft(body, body->position, old_pos, &wall_x))
	{
		body->position.x = wall_x + body->half_dim.x;
		TileBodySetCollision(body, TILE_LEFT);
	}

	wall_x = 0.0f;
	if (body->velocity.x >= 0.0f && TileBodyCheckRight(body, body->position, old_pos, &wall_x))
	{
		body->position.x = wall_x - body->half_dim.x;
		TileBodySetCollision(body, TILE_RIGHT);
	}
}

void TileBodyMoveY(TileBody* body, float y)
{
	vec2 old_pos = body->position;
	body->position.y += y;

	body->on_slope = 0;

	float ground_y = 0.0f;
	if (body->velocity.y <= 0.0f && TileBodyCheckBottom(body, body->position, old_pos, &ground_y))
	{
		body->position.y = ground_y + body->half_dim.y;
		TileBodySetCollision(body, TILE_BOTTOM);
	}

	ground_y = 0.0f;
	if (body->velocity.y >= 0.0f && TileBodyCheckTop(body, body->position, old_pos, &ground_y))
	{
		body->position.y = ground_y - body->half_dim.y;
		TileBodySetCollision(body, TILE_TOP);
	}
}

int TileBodyCheckBottom(TileBody* body, vec2 pos, vec2 old_pos, float* ground_y)
{
	line sensor = TileBodyGetSensor(body, TILE_BOTTOM, pos);
	line old_sensor = TileBodyGetSensor(body, TILE_BOTTOM, old_pos);

	float tile_size = body->map->tile_size;

	int32_t start_col = TileMapClamp(body->map, old_sensor.start.x);
	int32_t end_col = TileMapClamp(body->map, sensor.end.x);

	int32_t start_row = TileMapClamp(body->map, old_sensor.start.y);
	int32_t end_row = TileMapClamp(body->map, sensor.end.y + (body->slope_detected ? tile_size * 0.5f : 0.0f));

	if (start_row < 0 || end_row < 0)
	{
		*ground_y = 0.0f;
		return 1;
	}

	for (int32_t row = start_row; row >= end_row; --row)
	{
		for (int32_t col = start_col; col <= end_col; ++col)
		{
			Tile* tile = TileMapAt(body->map, row, col);
			if (!tile) continue;

			switch (tile->type)
			{
			case TILE_SOLID:
				if (body->on_slope) break;

				*ground_y = tile->pos.y + tile_size;
				return 1;
			case TILE_SLOPE_LEFT:
				*ground_y = tile->pos.y + tile_size - body->position.x + body->half_dim.x + tile->pos.x;
				body->on_slope = 1;
				return 1;
			case TILE_SLOPE_RIGHT:
				*ground_y = tile->pos.y + body->position.x + body->half_dim.x - tile->pos.x;
				body->on_slope = 1;
				return 1;
			case TILE_PLATFORM:
				if (body->drop) break;

				*ground_y = tile->pos.y + tile_size;
				if ((old_pos.y - body->half_dim.y) >= *ground_y)
					return 1;
			}
		}
	}

	return 0;
}

int TileBodyCheckTop(TileBody* body, vec2 pos, vec2 old_pos, float* ground_y)
{
	line sensor = TileBodyGetSensor(body, TILE_TOP, pos);
	line old_sensor = TileBodyGetSensor(body, TILE_TOP, old_pos);

	int32_t start_col = TileMapClamp(body->map, old_sensor.start.x);
	int32_t end_col = TileMapClamp(body->map, sensor.end.x);

	int32_t start_row = TileMapClamp(body->map, old_sensor.start.y);
	int32_t end_row = TileMapClamp(body->map, sensor.end.y);

	if (start_row >= body->map->height || end_row >= body->map->height) return 0;

	for (int32_t row = start_row; row <= end_row; ++row)
	{
		for (int32_t col = start_col; col <= end_col; ++col)
		{
			Tile* tile = TileMapAt(body->map, row, col);
			if (tile && tile->type == TILE_SOLID)
			{
				*ground_y = tile->pos.y;
				return 1;
			}
		}
	}

	return 0;
}

int TileBodyCheckLeft(TileBody* body, vec2 pos, vec2 old_pos, float* wall_x)
{
	line sensor = TileBodyGetSensor(body, TILE_LEFT, pos);
	line old_sensor = TileBodyGetSensor(body, TILE_LEFT, old_pos);

	int32_t start_col = TileMapClamp(body->map, old_sensor.start.x);
	int32_t end_col = TileMapClamp(body->map, sensor.end.x);

	int32_t start_row = TileMapClamp(body->map, old_sensor.start.y);
	int32_t end_row = TileMapClamp(body->map, sensor.end.y);

	if (start_col < 0 || end_col < 0) return 0;

	for (int32_t col = start_col; col >= end_col; --col)
	{
		for (int32_t row = start_row; row <= end_row; ++row)
		{
			Tile* tile = TileMapAt(body->map, row, col);
			if (tile && tile->type == TILE_SOLID)
			{
				*wall_x = tile->pos.x + body->map->tile_size;
				return 1;
			}
		}
	}

	return 0;
}

int TileBodyCheckRight(TileBody* body, vec2 pos, vec2 old_pos, float* wall_x)
{
	line sensor = TileBodyGetSensor(body, TILE_RIGHT, pos);
	line old_sensor = TileBodyGetSensor(body, TILE_RIGHT, old_pos);

	int32_t start_col = TileMapClamp(body->map, old_sensor.start.x);
	int32_t end_col = TileMapClamp(body->map, sensor.end.x);

	int32_t start_row = TileMapClamp(body->map, old_sensor.start.y);
	int32_t end_row = TileMapClamp(body->map, sensor.end.y);

	if (start_col >= body->map->width || end_col >= body->map->width) return 0;

	for (int32_t col = start_col; col <= end_col; ++col)
	{
		for (int32_t row = start_row; row <= end_row; ++row)
		{
			Tile* tile = TileMapAt(body->map, row, col);
			if (tile && tile->type == TILE_SOLID)
			{
				*wall_x = tile->pos.x;
				return 1;
			}
		}
	}

	return 0;
}

int TileBodyCheckSlope(const TileBody* body)
{
	float tile_size = body->map->tile_size;
	
	float near_x = 0.0f;
	float far_x = 0.0f;
	float y = 0.0f;
	TileType slope_type;

	if (body->velocity.x < 0.0f)
	{
		slope_type = TILE_SLOPE_LEFT;
		far_x = body->position.x - body->half_dim.x - tile_size + body->sensor_offset;
		near_x = body->position.x - body->half_dim.x - tile_size * 0.5f + body->sensor_offset;
		y = body->position.y + body->sensor_offset - body->half_dim.y;
	}
	else if (body->velocity.x > 0.0f)
	{
		slope_type = TILE_SLOPE_RIGHT;
		far_x = body->position.x + body->half_dim.x + tile_size - body->sensor_offset;
		near_x = body->position.x + body->half_dim.x + tile_size * 0.5f - body->sensor_offset;
		y = body->position.y + body->sensor_offset - body->half_dim.y;
	}
	else return 0;

	return TileMapCheckType(body->map, (vec2){far_x, y}, slope_type) || TileMapCheckType(body->map, (vec2){ near_x, y }, TILE_SLOPE_RIGHT);
}

void TileBodyResetCollision(TileBody* body)
{
	body->collision_state[TILE_LEFT] = 0;
	body->collision_state[TILE_RIGHT] = 0;
	body->collision_state[TILE_BOTTOM] = 0;
	body->collision_state[TILE_TOP] = 0;
}

void TileBodySetCollision(TileBody* body, TileDirection dir)
{
	if (dir >= 4 || dir < 0) return;

	body->collision_state[dir] = 1;

	if (dir == TILE_LEFT || dir == TILE_RIGHT)
		body->velocity.x = 0.0f;
	else if (dir == TILE_TOP || dir == TILE_BOTTOM)
		body->velocity.y = 0.0f;
}

void TileBodyResolveMap(TileBody* body, vec2 gravity, float deltatime)
{
	float grav = body->gravity_scale * deltatime;
	if (body->on_slope && body->velocity.y <= 0.0f)
		grav *= TILE_SLOPE_GRIP;

	body->velocity.x += gravity.x * grav;
	body->velocity.y += gravity.y * grav;

	// check for slopes
	body->slope_detected = TileBodyCheckSlope(body);

	TileBodyResetCollision(body);
	
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
			TileBodySetCollision(body, TILE_LEFT);
		}
		else if (body->velocity.x > 0.0f)
		{
			body->position.x -= tb_max_f(overlap_x, 0.0f);
			TileBodySetCollision(body, TILE_RIGHT);
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
			TileBodySetCollision(body, TILE_BOTTOM);
		}
		else if (body->velocity.y > 0.0f)
		{
			body->position.y -= tb_max_f(overlap_y, 0.0f);
			TileBodySetCollision(body, TILE_TOP);
		}
	}
}

line TileBodyGetSensor(const TileBody* body, TileDirection dir, vec2 pos)
{
	line l;
	vec2 offset;
	switch (dir)
	{
	case TILE_LEFT:
		offset = (vec2){ body->sensor_offset, body->on_slope ? 12.0f : 2.0f };
		l.start = (vec2){ pos.x - body->half_dim.x - offset.x, pos.y - body->half_dim.y + offset.y };
		l.end = (vec2){ pos.x - body->half_dim.x - offset.x, pos.y + body->half_dim.y - offset.y };
		break;
	case TILE_RIGHT:
		offset = (vec2){ body->sensor_offset, body->on_slope ? 12.0f : 2.0f };
		l.start = (vec2){ pos.x + body->half_dim.x + offset.x, pos.y - body->half_dim.y + offset.y };
		l.end = (vec2){ pos.x + body->half_dim.x + offset.x, pos.y + body->half_dim.y - offset.y };
		break;
	case TILE_TOP:
		offset = (vec2){ 2.0f, body->sensor_offset };
		l.start = (vec2){ pos.x - body->half_dim.x + offset.x, pos.y + body->half_dim.y + offset.y };
		l.end = (vec2){ pos.x + body->half_dim.x - offset.x, pos.y + body->half_dim.y + offset.y };
		break;
	case TILE_BOTTOM:
		offset = (vec2){ 2.0f, body->sensor_offset };
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
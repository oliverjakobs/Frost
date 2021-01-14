#include "tile_collision.h"

int TileCheckBottom(TileBody* body, const TileMap* map, vec2 pos, vec2 old_pos, float* ground_y)
{
	line sensor = TileBodyGetSensor(body, TILE_BOTTOM, pos);
	line old_sensor = TileBodyGetSensor(body, TILE_BOTTOM, old_pos);

	float tile_size = map->tile_size;

	int32_t start_col = TileMapClamp(map, sensor.start.x);
	int32_t end_col = TileMapClamp(map, old_sensor.end.x);

	int32_t start_row = TileMapClamp(map, old_sensor.end.y + (body->slope_detected ? tile_size * 0.5f : 0.0f));
	int32_t end_row = TileMapClamp(map, sensor.start.y);

	if (start_row < 0 || end_row < 0)
	{
		*ground_y = 0.0f;
		return TileMapGetBorder(map, TILE_BOTTOM);
	}

	for (int32_t row = start_row; row >= end_row; --row)
	{
		TileType collision_type = TILE_EMPTY;
		for (int32_t col = start_col; col <= end_col; ++col)
		{
			Tile* tile = TileMapAt(map, row, col);
			if (!tile || (collision_type != TILE_EMPTY && collision_type <= tile->type)) continue;

			switch (tile->type)
			{
			case TILE_SOLID:
				if (!body->on_slope)
				{
					*ground_y = tile->pos.y + tile_size;
					collision_type = TILE_SOLID;
				}
				break;
			case TILE_SLOPE_LEFT:
				*ground_y = tile->pos.y + tile_size - body->position.x + body->half_dim.x + tile->pos.x;
				collision_type = TILE_SLOPE_LEFT;
				break;
			case TILE_SLOPE_RIGHT:
				*ground_y = tile->pos.y + body->position.x + body->half_dim.x - tile->pos.x;
				collision_type = TILE_SLOPE_RIGHT;
				break;
			case TILE_PLATFORM:
				if (!body->drop)
				{
					*ground_y = tile->pos.y + tile_size;
					if ((old_pos.y - body->half_dim.y) >= *ground_y)
						collision_type = TILE_PLATFORM;
				}
				break;
			}
		}
		if (collision_type != TILE_EMPTY)
		{
			if (collision_type == TILE_SLOPE_RIGHT || collision_type == TILE_SLOPE_LEFT)
				body->on_slope = 1;

			return 1;
		}
	}

	return 0;
}

int TileCheckTop(TileBody* body, const TileMap* map, vec2 pos, vec2 old_pos, float* ground_y)
{
	line sensor = TileBodyGetSensor(body, TILE_TOP, pos);
	line old_sensor = TileBodyGetSensor(body, TILE_TOP, old_pos);

	int32_t start_col = TileMapClamp(map, old_sensor.start.x);
	int32_t end_col = TileMapClamp(map, sensor.end.x);

	int32_t start_row = TileMapClamp(map, old_sensor.start.y);
	int32_t end_row = TileMapClamp(map, sensor.end.y);

	if (start_row >= map->rows || end_row >= map->rows)
	{
		*ground_y = map->rows * map->tile_size;
		return TileMapGetBorder(map, TILE_TOP);
	}

	for (int32_t row = start_row; row <= end_row; ++row)
	{
		for (int32_t col = start_col; col <= end_col; ++col)
		{
			Tile* tile = TileMapAt(map, row, col);
			if (tile && tile->type == TILE_SOLID)
			{
				*ground_y = tile->pos.y;
				return 1;
			}
		}
	}

	return 0;
}

int TileCheckLeft(TileBody* body, const TileMap* map, vec2 pos, vec2 old_pos, float* wall_x)
{
	line sensor = TileBodyGetSensor(body, TILE_LEFT, pos);
	line old_sensor = TileBodyGetSensor(body, TILE_LEFT, old_pos);

	int32_t start_col = TileMapClamp(map, old_sensor.start.x);
	int32_t end_col = TileMapClamp(map, sensor.end.x);

	int32_t start_row = TileMapClamp(map, old_sensor.start.y);
	int32_t end_row = TileMapClamp(map, sensor.end.y);

	if (start_col < 0 || end_col < 0)
	{
		*wall_x = 0.0f;
		return TileMapGetBorder(map, TILE_LEFT);
	}

	for (int32_t col = start_col; col >= end_col; --col)
	{
		for (int32_t row = start_row; row <= end_row; ++row)
		{
			Tile* tile = TileMapAt(map, row, col);
			if (tile && tile->type == TILE_SOLID)
			{
				*wall_x = tile->pos.x + map->tile_size;
				return 1;
			}
		}
	}

	return 0;
}

int TileCheckRight(TileBody* body, const TileMap* map, vec2 pos, vec2 old_pos, float* wall_x)
{
	line sensor = TileBodyGetSensor(body, TILE_RIGHT, pos);
	line old_sensor = TileBodyGetSensor(body, TILE_RIGHT, old_pos);

	int32_t start_col = TileMapClamp(map, old_sensor.start.x);
	int32_t end_col = TileMapClamp(map, sensor.end.x);

	int32_t start_row = TileMapClamp(map, old_sensor.start.y);
	int32_t end_row = TileMapClamp(map, sensor.end.y);

	if (start_col >= map->cols || end_col >= map->cols)
	{
		*wall_x = map->cols * map->tile_size;
		return TileMapGetBorder(map, TILE_RIGHT);
	}

	for (int32_t col = start_col; col <= end_col; ++col)
	{
		for (int32_t row = start_row; row <= end_row; ++row)
		{
			Tile* tile = TileMapAt(map, row, col);
			if (tile && tile->type == TILE_SOLID)
			{
				*wall_x = tile->pos.x;
				return 1;
			}
		}
	}

	return 0;
}

int TileCheckSlope(const TileBody* body, const TileMap* map)
{
	float tile_size = map->tile_size;

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

	return TileMapCheckType(map, (vec2) { far_x, y }, slope_type) || TileMapCheckType(map, (vec2) { near_x, y }, slope_type);
}

void TileResolveBodies(TileBody* body, const TileBody* other, vec2 old_pos)
{
	float x0 = (body->position.x + body->half_dim.x) - (other->position.x - other->half_dim.x);
	float x1 = (other->position.x + other->half_dim.x) - (body->position.x - body->half_dim.x);

	float y0 = (old_pos.y + body->half_dim.y) - (other->position.y - other->half_dim.y);
	float y1 = (other->position.y + other->half_dim.y) - (old_pos.y - body->half_dim.y);

	float overlap_x = x0 < x1 ? x0 : x1;
	float overlap_y = y0 < y1 ? y0 : y1;

	// horizontal resolve
	if (overlap_x > 0.0f && overlap_y > 0.0f)
	{
		if (body->velocity.x < 0.0f)
		{
			body->position.x += overlap_x;
			TileBodySetCollision(body, TILE_LEFT);
		}
		else if (body->velocity.x > 0.0f)
		{
			body->position.x -= overlap_x;
			TileBodySetCollision(body, TILE_RIGHT);
		}
	}

	x0 = (old_pos.x + body->half_dim.x) - (other->position.x - other->half_dim.x);
	x1 = (other->position.x + other->half_dim.x) - (old_pos.x - body->half_dim.x);

	y0 = (body->position.y + body->half_dim.y) - (other->position.y - other->half_dim.y);
	y1 = (other->position.y + other->half_dim.y) - (body->position.y - body->half_dim.y);

	overlap_x = x0 < x1 ? x0 : x1;
	overlap_y = y0 < y1 ? y0 : y1;

	// vertical resolve
	if (overlap_x > 0.0f && overlap_y > 0.0f)
	{
		if (body->velocity.y < 0.0f)
		{
			body->position.y += overlap_y;
			TileBodySetCollision(body, TILE_BOTTOM);
		}
		else if (body->velocity.y > 0.0f)
		{
			body->position.y -= overlap_y;
			TileBodySetCollision(body, TILE_TOP);
		}
	}
}
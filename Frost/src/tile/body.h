#ifndef TILE_BODY_H
#define TILE_BODY_H

#include "tile.h"
#include "tilemap.h"

typedef struct
{
    vec2 position;
    vec2 half_dim;

    vec2 velocity;

	// -----------------------------------
	float sensor_offset;	// offset to the edges
	vec2 offset_horizontal;
	vec2 offset_vertical;

	// CollisionState
	int collides_bottom;
	int collides_top;
	int collides_left;
	int collides_right;

	// Slopes
	int on_slope;
	int slope_detected;

	float gravity_scale;	// amplifies the magnitude of the gravity
	int drop;				// should the body drop through a platform

	const TileMap* map;
} TileBody;

void TileBodyInit(TileBody* body, const TileMap* map, float x, float y, float h_w, float h_h);

void TileBodyMoveX(TileBody* body, float x);
void TileBodyMoveY(TileBody* body, float y);

int TileBodyCheckBottom(TileBody* body, vec2 pos, vec2 old_pos, float* ground_y);
int TileBodyCheckTop(TileBody* body, vec2 pos, vec2 old_pos, float* ground_y);
int TileBodyCheckLeft(TileBody* body, vec2 pos, vec2 old_pos, float* wall_x);
int TileBodyCheckRight(TileBody* body, vec2 pos, vec2 old_pos, float* wall_x);

void TileBodyResolveMap(TileBody* body, vec2 gravity, float deltatime);
void TileBodyResolveBody(TileBody* body, const TileBody* other, vec2 old_pos);

line TileBodyGetSensor(const TileBody* body, TileDirection dir, vec2 pos, vec2 offset);

#endif // !TILE_BODY_H

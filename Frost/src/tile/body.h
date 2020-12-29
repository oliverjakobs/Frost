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
} TileBody;

void TileBodyInit(TileBody* body, float x, float y, float h_w, float h_h);

void TileBodyMoveX(TileBody* body, float x);
void TileBodyMoveY(TileBody* body, float y);

int TileBodyCheckBottom(const TileBody* body, vec2 pos, vec2 old_pos, float* ground_y);
int TileBodyCheckTop(const TileBody* body, vec2 pos, vec2 old_pos, float* ground_y);
int TileBodyCheckLeft(const TileBody* body, vec2 pos, vec2 old_pos, float* wall_x);
int TileBodyCheckRight(const TileBody* body, vec2 pos, vec2 old_pos, float* wall_y);

void TileBodyResolveMap(TileBody* body, TileMap* map, vec2 gravity, float deltatime);
void TileBodyResolveBody(TileBody* body, TileBody* other, vec2 old_pos);

float TileBodyGetX(const TileBody* body);
float TileBodyGetY(const TileBody* body);
float TileBodyGetX2(const TileBody* body);
float TileBodyGetY2(const TileBody* body);

#endif // !TILE_BODY_H

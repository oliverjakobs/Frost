#ifndef TILE_BODY_H
#define TILE_BODY_H

#include "tile.h"

struct TileBody
{
    vec2 position;
    vec2 half_dim;

    vec2 velocity;

	TileBodyType type;

	// -----------------------------------
	float gravity_scale;	// amplifies the magnitude of the gravity
	float sensor_offset;	// offset to the edges

	// CollisionState
	int collision_state[4];
	int on_slope;
	int slope_detected;
	int drop;				// should the body drop through a platform

	const TileMap* map;
};

void TileBodyInit(TileBody* body, const TileMap* map, TileBodyType type, float x, float y, float h_w, float h_h);

void TileBodyMoveX(TileBody* body, float x);
void TileBodyMoveY(TileBody* body, float y);

void TileBodyResetCollision(TileBody* body);
void TileBodySetCollision(TileBody* body, TileDirection dir);

void TileBodyApplyGravity(TileBody* body, vec2 gravity, float slope_grip, float deltatime);
void TileBodyTick(TileBody* body, float deltatime);

line TileBodyGetSensor(const TileBody* body, TileDirection dir, vec2 pos);

#endif // !TILE_BODY_H

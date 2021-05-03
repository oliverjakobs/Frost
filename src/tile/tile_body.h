#ifndef TILE_BODY_H
#define TILE_BODY_H

#include "tile_map.h"

typedef enum
{
	TILE_BODY_STATIC,
	TILE_BODY_DYNAMIC
} TileBodyType;

typedef struct
{
    vec2 position;
    vec2 half_dim;

    vec2 velocity;

	TileBodyType type;

	/* ----------------------------------- */
	float gravity_scale;	/* amplifies the magnitude of the gravity */
	float sensor_offset;	/* offset of the sensor to the edges */
	float sensor_padding;

	int collision_state[4];
	int on_slope;
	int slope_detected;
	int drop;				/* should the body drop through a platform */
} TileBody;

void TileBodyInit(TileBody* body, TileBodyType type, float x, float y, float h_w, float h_h);
void TileBodySetSensor(TileBody* body, float offset, float padding);

void TileBodyMoveX(TileBody* body, const TileMap* map, float x);
void TileBodyMoveY(TileBody* body, const TileMap* map, float y);

void TileBodyResetCollision(TileBody* body);
void TileBodySetCollision(TileBody* body, TileDirection dir);

void TileBodyApplyGravity(TileBody* body, vec2 gravity, float slope_grip, float deltatime);
void TileBodyTick(TileBody* body, const TileMap* map, float deltatime);

line TileBodyGetSensor(const TileBody* body, TileDirection dir, vec2 pos);

#endif /* !TILE_BODY_H */

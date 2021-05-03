#ifndef TILE_COLLISION_H
#define TILE_COLLISION_H

#include "tile_body.h"

int TileCheckBottom(TileBody* body, const TileMap* map, vec2 pos, vec2 old_pos, float* ground_y);
int TileCheckTop(TileBody* body, const TileMap* map, vec2 pos, vec2 old_pos, float* ground_y);
int TileCheckLeft(TileBody* body, const TileMap* map, vec2 pos, vec2 old_pos, float* wall_x);
int TileCheckRight(TileBody* body, const TileMap* map, vec2 pos, vec2 old_pos, float* wall_x);

int TileCheckSlope(const TileBody* body, const TileMap* map);

void TileResolveBodies(TileBody* body, const TileBody* other, vec2 old_pos);

#endif /* !TILE_COLLISION_H */

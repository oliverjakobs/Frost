#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "tile/tile_body.h"

#include "Scenes/Scene.h"

/*
REQUIRES:
  - transform

PARSE_RULES:

TileBodyType:
  "DYNAMIC" -> TILE_BODY_DYNAMIC
  default   -> TILE_BODY_STATIC

RigidBodyFilter:
  "WORLD"   -> RIGID_BODY_FILTER_WORLD
  "PLAYER"  -> RIGID_BODY_FILTER_PLAYER
  "NPC"     -> RIGID_BODY_FILTER_NPC
  "DOOR"    -> RIGID_BODY_FILTER_DOOR
  default   -> RIGID_BODY_FILTER_NONE

Template:

[rigidbody]
type = <TileBodyType>
filter = { <RigidBodyFilter>, ... }
half_width = <float : transform.width / 2>
half_height = <float : transform.height / 2>
offset_x = <float : 0.0>
offset_y = <float : half_height>
*/

typedef enum
{
    RIGID_BODY_FILTER_NONE      = 0,        /* 00000000 */
    RIGID_BODY_FILTER_WORLD     = 1 << 0,   /* 00000001 */
    RIGID_BODY_FILTER_PLAYER    = 1 << 1,   /* 00000010	*/
    RIGID_BODY_FILTER_NPC       = 1 << 2,   /* 00000100	*/
    RIGID_BODY_FILTER_DOOR      = 1 << 3,   /* 00001000	*/
    RIGID_BODY_FILTER_3         = 1 << 4,   /* 00010000	*/
    RIGID_BODY_FILTER_4         = 1 << 5,   /* 00100000	*/
    RIGID_BODY_FILTER_5         = 1 << 6,   /* 01000000	*/
    RIGID_BODY_FILTER_6         = 1 << 7    /* 10000000	*/
} RigidBodyFilter;

typedef struct
{
    TileBody body;

    vec2 offset;
    RigidBodyFilter filter;
} RigidBody;

void RigidBodyLoad(char* ini, Scene* scene, EcsEntityID entity);

#endif /* !RIGID_BODY_H */
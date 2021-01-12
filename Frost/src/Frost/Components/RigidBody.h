#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "tile/body.h"

#include "Scenes/Scene.h"

/*
REQUIRES:
	- transform

PARSE_RULES:

TileBodyType:
	"DYNAMIC"	-> TILE_BODY_DYNAMIC
	default		-> TILE_BODY_STATIC

RigidBodyFilter:
	"WORLD"		-> RIGID_BODY_FILTER_WORLD
	"PLAYER"	-> RIGID_BODY_FILTER_PLAYER
	"NPC"		-> RIGID_BODY_FILTER_NPC
	"DOOR"		-> RIGID_BODY_FILTER_DOOR
	default		-> RIGID_BODY_FILTER_NONE

JSON - Template:

"rigidbody": {
	"type": <TileBodyType>,
	"filter": [ <RigidBodyFilter>, ... ],
	<OPTIONAL (default transform size / 2)>
		"halfsize": [ <HALF_WIDTH:float>, <HALF_HEIGHT:float> ],
	<OPTIONAL (default [ 0.0f, HALF_HEIGHT ])>
		"offset": [ <X:float>, <Y:float> ],
}
*/

typedef enum
{
	RIGID_BODY_FILTER_NONE		= 0,		/* 00000000 */
	RIGID_BODY_FILTER_WORLD		= 1 << 0,	/* 00000001 */
	RIGID_BODY_FILTER_PLAYER	= 1 << 1,	/* 00000010	*/
	RIGID_BODY_FILTER_NPC		= 1 << 2,	/* 00000100	*/
	RIGID_BODY_FILTER_DOOR		= 1 << 3,	/* 00001000	*/
	RIGID_BODY_FILTER_3			= 1 << 4,	/* 00010000	*/
	RIGID_BODY_FILTER_4			= 1 << 5,	/* 00100000	*/
	RIGID_BODY_FILTER_5			= 1 << 6,	/* 01000000	*/
	RIGID_BODY_FILTER_6			= 1 << 7	/* 10000000	*/
} RigidBodyFilter;

typedef struct
{
	TileBody body;

	vec2 offset;
	RigidBodyFilter filter;
} RigidBody;

void RigidBodyLoad(Scene* scene, EcsEntityID entity, char* json);

TileBodyType RigidBodyParseType(const char* str, size_t max_count);
RigidBodyFilter RigidBodyParseFilter(const char* str, size_t max_count);

#endif /* !RIGID_BODY_H */
#ifndef SPRITE_H
#define SPRITE_H

#include "Ignis/Ignis.h"

typedef enum
{
	SPRITE_FLIP_NONE = 0,
	SPRITE_FLIP_HORIZONTAL,
	SPRITE_FLIP_VERTICAL,
	SPRITE_FLIP_BOTH
} SpriteFlip;

typedef struct
{
	IgnisTexture2D* texture;
	size_t frame;

	float width;
	float height;

	SpriteFlip flip;
} Sprite;

#endif /* !SPRITE_H */
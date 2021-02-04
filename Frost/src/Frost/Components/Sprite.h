#ifndef SPRITE_H
#define SPRITE_H

#include "Ignis/Ignis.h"

#include "Scenes/Scene.h"

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

void SpriteLoad(char* json, Ecs* ecs, EcsEntityID entity, const Resources* res, int z_index, int variant);

#endif /* !SPRITE_H */
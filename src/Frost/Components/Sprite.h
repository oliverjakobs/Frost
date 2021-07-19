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

void SpriteLoad(char* ini, Ecs* ecs, EcsEntityID entity, const Resources* res, int z_index, int variant);

void SpriteGetSrcRect(const Sprite* sprite, float* src_x, float* src_y, float* src_w, float* src_h);

#endif /* !SPRITE_H */
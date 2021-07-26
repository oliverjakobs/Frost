#ifndef SPRITE_H
#define SPRITE_H

#include "Ignis/Ignis.h"

#include "Scenes/Scene.h"

/*
REQUIRES:
  - transform

TEMPLATE:
[sprite]
path = <str>
width = <float : transform.width>
height = <float : transform.height>
frame = <int | VARIANT : 0> 
rows = <int : 1>
cols = <int : 1>
*/

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

void SpriteLoad(char* ini, Scene* scene, EcsEntityID entity, int z_index, int variant);

void SpriteGetSrcRect(const Sprite* sprite, float* src_x, float* src_y, float* src_w, float* src_h);

#endif /* !SPRITE_H */
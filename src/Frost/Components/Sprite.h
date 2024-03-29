#ifndef SPRITE_H
#define SPRITE_H

#include "ignis/ignis.h"

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
    uint32_t frame;

    float width;
    float height;

    int rows, cols;

    SpriteFlip flip;
} Sprite;

void SpriteLoad(char* ini, Scene* scene, EcsEntityID entity, int z_index, int variant);

IgnisRect SpriteGetSrcRect(const Sprite* sprite);

#endif /* !SPRITE_H */
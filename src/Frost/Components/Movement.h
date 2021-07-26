#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "Scenes/Scene.h"

/*
REQUIRES:
  - transform

TEMPLATE:
[movement]
speed = <float : 0.0>
bounce = <float : 0.0>
*/

typedef enum
{
    MOVEMENT_RIGHT = 0,
    MOVEMENT_DOWN,
    MOVEMENT_LEFT,
    MOVEMENT_UP
} MovementDirection;

typedef struct
{
    MovementDirection direction;
    float speed;
    float bounce;
} Movement;

void MovementLoad(char* ini, Scene* scene, EcsEntityID entity);

#endif /* !MOVEMENT_H */

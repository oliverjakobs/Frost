#ifndef PLAYER_H
#define PLAYER_H

#include "Scenes/Scene.h"

/*
TEMPLATE:
[player]
*/

typedef struct
{
    int move_left;
    int move_right;
    int jump;
} Player;

void PlayerLoad(char* ini, Scene* scene, EcsEntityID entity);

#endif /* !PLAYER_H */
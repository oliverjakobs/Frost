#ifndef PLAYER_H
#define PLAYER_H

#include "Scenes/Scene.h"

typedef struct
{
    int move_left;
    int move_right;
    int jump;
} Player;

void PlayerLoad(char* ini, Ecs* ecs, EcsEntityID entity);

#endif /* !PLAYER_H */
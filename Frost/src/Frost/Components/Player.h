#ifndef PLAYER_H
#define PLAYER_H

#include "Scenes/Scene.h"

typedef struct
{
    int move_left;
    int move_right;
    int jump;
} Player;

void PlayerLoad(Scene* scene, EcsEntityID entity, char* json);

#endif /* !PLAYER_H */
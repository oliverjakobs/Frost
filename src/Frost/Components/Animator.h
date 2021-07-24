#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Scenes/Scene.h"
#include "EntityState.h"

/*
REQUIRES:
  - sprite

TEMPLATE(grouped):
[animation.<NAME>]
start = <int : 0>
length = <int : 0>
delay = <float : 0.0>
*/

typedef struct
{
    int start;
    int length;

    float delay;
} Animation;

typedef struct
{
    Animation animations[NUM_ENTITY_STATES];
    EntityState current;
    int frame;

    float clock;
} Animator;

void AnimatorLoad(char* ini, Ecs* ecs, EcsEntityID entity);
void AnimatorRelease(Animator* animator);

void AnimatorStart(Animator* animator, int start);

#endif /* !ANIMATOR_H */

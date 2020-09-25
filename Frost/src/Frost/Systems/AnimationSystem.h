#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "Ecs/Ecs.h"

typedef struct
{
	int (*func)(Ecs*, EcsEntityID, int);
} AnimationConditionCallback;

int AnimationSystemInit();
void AnimationSystemDestroy();

int AnimationSystemAddCondition(int (*condition)(Ecs*, EcsEntityID, int));
AnimationConditionCallback* AnimationSystemGetCondition(size_t index);

void AnimationSystem(Ecs* ecs, float deltatime);

/* animation contitions */
int AnimationConditionJump(Ecs* ecs, EcsEntityID e, int s);
int AnimationConditionFall(Ecs* ecs, EcsEntityID e, int s);
int AnimationConditionWalk(Ecs* ecs, EcsEntityID e, int s);
int AnimationConditionIdle(Ecs* ecs, EcsEntityID e, int s);

#endif /* !ANIMATION_SYSTEM_H */
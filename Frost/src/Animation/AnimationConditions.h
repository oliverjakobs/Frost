#ifndef ANIMATION_CONDITIONS_H
#define ANIMATION_CONDITIONS_H

#include "ECS/Ecs.h"

typedef struct
{
	int (*func)(Ecs*, EntityID, int);
} AnimationCondition;

void AnimationConditionsInit();
void AnimationConditionsDestroy();

int AnimationConditionsRegisterCondition(const char* name, int (*condition)(Ecs*,EntityID,int));
AnimationCondition* AnimationConditionsGetCondition(const char* name);

int AnimationConditionJump(Ecs* ecs, EntityID e, int s);
int AnimationConditionFall(Ecs* ecs, EntityID e, int s);
int AnimationConditionWalk(Ecs* ecs, EntityID e, int s);
int AnimationConditionIdle(Ecs* ecs, EntityID e, int s);

#endif /* !ANIMATION_CONDITIONS_H */

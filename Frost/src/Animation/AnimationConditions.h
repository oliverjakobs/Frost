#ifndef ANIMATION_CONDITIONS_H
#define ANIMATION_CONDITIONS_H

#include "ECS/ComponentTable.h"

typedef struct
{
	int (*func)(ComponentTable*, EntityID, int);
} AnimationCondition;

void AnimationConditionsInit();
void AnimationConditionsDestroy();

int AnimationConditionsRegisterCondition(const char* name, int (*condition)(ComponentTable*,EntityID,int));
AnimationCondition* AnimationConditionsGetCondition(const char* name);

int AnimationConditionJump(ComponentTable* components, EntityID e, int s);
int AnimationConditionFall(ComponentTable* components, EntityID e, int s);
int AnimationConditionWalk(ComponentTable* components, EntityID e, int s);
int AnimationConditionIdle(ComponentTable* components, EntityID e, int s);

#endif /* !ANIMATION_CONDITIONS_H */

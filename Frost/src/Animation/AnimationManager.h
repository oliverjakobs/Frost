#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include "ECS/ComponentTable.h"

typedef struct
{
	int (*func)(ComponentTable*, const char*, int);
} AnimationCondition;

void AnimationManagerInit();
void AnimationManagerDestroy();

int AnimationManagerRegisterCondition(const char* name, int (*condition)(ComponentTable*, const char*, int));
AnimationCondition* AnimationManagerGetCondition(const char* name);

#endif /* !ANIMATION_MANAGER_H */
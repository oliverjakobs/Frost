#include "AnimationManager.h"

#include <stdlib.h>

typedef struct
{
	clib_hashmap conditions;	/* <str, (int (*condition)(const char*, int))> */
} AnimationManager;

static AnimationManager manager;

void AnimationManagerInit()
{
	clib_dict_alloc(&manager.conditions, 0);
}

void AnimationManagerDestroy()
{
	CLIB_DICT_ITERATE_FOR(&manager.conditions, iter)
	{
		free(clib_dict_iter_get_value(iter));
		clib_dict_iter_remove(&manager.conditions, iter);
	}
}

int AnimationManagerRegisterCondition(const char* name, int(*condition)(ComponentTable*, const char*, int))
{
	AnimationCondition* value = (AnimationCondition*)malloc(sizeof(AnimationCondition));

	if (value)
	{
		value->func = condition;
		if (clib_dict_insert(&manager.conditions, name, value) == value)
			return 1;
	}

	free(value);
	return 0;
}

AnimationCondition* AnimationManagerGetCondition(const char* name)
{
	return clib_dict_find(&manager.conditions, name);
}

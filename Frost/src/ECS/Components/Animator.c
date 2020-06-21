#include "Animator.h"

#include <stdlib.h>

void AnimatorInit(Animator* animator)
{
	clib_dict_alloc(&animator->animations, 0);

	animator->current = NULL;
}

void AnimatorDestroy(Animator* animator)
{
	CLIB_DICT_ITERATE_FOR(&animator->animations, iter)
	{
		free(clib_dict_iter_get_value(iter));
		clib_dict_iter_remove(&animator->animations, iter);
	}
}

int AnimatorAddAnimation(Animator* animator, const char* name, Animation* animation)
{
	if (clib_dict_insert(&animator->animations, name, animation) != animation)
	{
		AnimationDestroy(animation);
		free(animation);
		return 0;
	}

	if (animator->current == NULL)
		animator->current = (char*)clib_dict_get_key_ptr(&animator->animations, name);

	return 1;
}

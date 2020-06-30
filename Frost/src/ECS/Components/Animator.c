#include "Animator.h"

#include <stdlib.h>

void AnimatorFree(void* block)
{
	CLIB_DICT_ITERATE_FOR(&((Animator*)block)->animations, iter)
	{
		free(clib_dict_iter_get_value(iter));
		clib_dict_iter_remove(&((Animator*)block)->animations, iter);
	}
	clib_dict_free(&((Animator*)block)->animations);
	free(block);
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

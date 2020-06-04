#include "Animator.h"

#include "AnimationConditions.h"

typedef struct
{
	int (*func)(struct ecs_entity*, int);
} AnimationCondition;

/* Declare type-specific type_dict_* functions with this handy macro */
CLIB_DICT_DEFINE_FUNCS(condition, AnimationCondition)
CLIB_DICT_DEFINE_FUNCS(animation, Animation)

void AnimatorInit(Animator* animator)
{
	clib_dict_init(&animator->conditions, 0);
	clib_dict_init(&animator->animations, 0);

	AnimatorRegisterConition(animator, "condition_jump", AnimationConditionJump);
	AnimatorRegisterConition(animator, "condition_fall", AnimationConditionFall);
	AnimatorRegisterConition(animator, "condition_walk", AnimationConditionWalk);
	AnimatorRegisterConition(animator, "condition_idle", AnimationConditionIdle);

	animator->current = NULL;
}

void AnimatorDestroy(Animator* animator)
{
	for (clib_dict_iter* iter = clib_dict_iterator(&animator->animations); iter; iter = clib_dict_iter_next(&animator->animations, iter))
	{
		free(animation_dict_iter_get_value(iter));
		clib_dict_iter_remove(&animator->animations, iter);
	}

	for (clib_dict_iter* iter = clib_dict_iterator(&animator->conditions); iter; iter = clib_dict_iter_next(&animator->conditions, iter))
	{
		free(condition_dict_iter_get_value(iter));
		clib_dict_iter_remove(&animator->conditions, iter);
	}
}

int AnimatorRegisterConition(Animator* animator, const char* name, int (*condition)(struct ecs_entity*, int))
{
	AnimationCondition* value = (AnimationCondition*)malloc(sizeof(AnimationCondition));

	if (value)
	{
		value->func = condition;
		if (condition_dict_insert(&animator->conditions, name, value) == value)
			return 1;
	}

	free(value);
	return 0;
}

int AnimatorAddAnimation(Animator* animator, const char* name, Animation* animation)
{
	if (animation_dict_insert(&animator->animations, name, animation) != animation)
	{
		AnimationDestroy(animation);
		free(animation);
		return 0;
	}

	if (animator->current == NULL)
		animator->current = clib_dict_get_key_ptr(&animator->animations, name);

	return 1;
}

void AnimatorTick(Animator* animator, struct ecs_entity* entity, float deltatime)
{
	if (!animator->current)
		return;

	Animation* animation = animation_dict_get(&animator->animations, animator->current);
	if (animation)
	{
		AnimationTick(animation, deltatime);
		clib_strmap* transitions = &animation->transitions;
		for (clib_strmap_iter* iter = clib_strmap_iterator(transitions); iter; iter = clib_strmap_iter_next(transitions, iter))
		{
			AnimationCondition* cond = condition_dict_get(&animator->conditions, clib_strmap_iter_get_key(iter));
			if (cond && cond->func(entity, 0))
				AnimatorPlay(animator, clib_strmap_iter_get_value(iter));
		}
	}
}

void AnimatorPlay(Animator* animator, const char* name)
{
	if (strcmp(animator->current, name) == 0)
		return;

	Animation* animation = animation_dict_get(&animator->animations, name);
	if (animation)
	{
		AnimationStart(animation);
		animator->current = clib_dict_get_key_ptr(&animator->animations, name);
	}
}

int AnimatorGetFrame(Animator* animator)
{
	if (!animator->current) return 0;

	Animation* animation = animation_dict_get(&animator->animations, animator->current);
		
	return animation ? animation->frame : 0;
}

void AnimatorDebugPrint(Animator* animator)
{
	for (clib_dict_iter* iter = clib_dict_iterator(&animator->conditions); iter; iter = clib_dict_iter_next(&animator->conditions, iter))
		printf("Condition: %s\n", clib_dict_iter_get_key(iter));

	for (clib_dict_iter* iter = clib_dict_iterator(&animator->animations); iter; iter = clib_dict_iter_next(&animator->animations, iter))
	{
		printf("Animation: %s\n", clib_dict_iter_get_key(iter));

		Animation* animation = animation_dict_iter_get_value(iter);

		for (clib_dict_iter* a_iter = clib_dict_iterator(&animation->transitions); a_iter; a_iter = clib_dict_iter_next(&animation->transitions, a_iter))
			printf("Transition: %s - %s\n", clib_strmap_iter_get_key(a_iter), clib_strmap_iter_get_value(a_iter));
	}
}

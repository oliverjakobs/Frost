#include "Animator.h"

#include "AnimationConditions.h"

typedef struct
{
	char key[ANIMATION_STRLEN];
	int (*condition)(EcsEntity*, int);
} _conditionkvp;

typedef struct
{
	char key[ANIMATION_STRLEN];
	Animation* animation;
} _animationkvp;

/* Declare type-specific type_hashmap_* functions with this handy macro */
CLIB_HASHMAP_DEFINE_FUNCS(condition, char, _conditionkvp)
CLIB_HASHMAP_DEFINE_FUNCS(animation, char, _animationkvp)

_conditionkvp* _condition_kvp(const char* key, int (*condition)(EcsEntity*, int))
{
	if (strlen(key) > ANIMATION_STRLEN)
		return NULL;

	_conditionkvp* kvp = (_conditionkvp*)malloc(sizeof(_conditionkvp));

	if (kvp)
	{
		strncpy(kvp->key, key, ANIMATION_STRLEN);
		kvp->condition = condition;
	}

	return kvp;
}

_animationkvp* _animation_kvp(const char* key, Animation* animation)
{
	if (strlen(key) > ANIMATION_STRLEN)
		return NULL;

	_animationkvp* kvp = (_animationkvp*)malloc(sizeof(_animationkvp));

	if (kvp)
	{
		strncpy(kvp->key, key, ANIMATION_STRLEN);
		kvp->animation = animation;
	}

	return kvp;
}

void AnimatorInit(Animator* animator)
{
	clib_hashmap_init(&animator->conditions, clib_hashmap_hash_string, clib_hashmap_compare_string, 0);
	clib_hashmap_init(&animator->animations, clib_hashmap_hash_string, clib_hashmap_compare_string, 0);

	AnimatorRegisterConition(animator, "condition_jump", AnimationConditionJump);
	AnimatorRegisterConition(animator, "condition_fall", AnimationConditionFall);
	AnimatorRegisterConition(animator, "condition_walk", AnimationConditionWalk);
	AnimatorRegisterConition(animator, "condition_idle", AnimationConditionIdle);

	animator->current = NULL;
}

void AnimatorDestroy(Animator* animator)
{
	for (clib_hashmap_iter* iter = clib_hashmap_iterator(&animator->animations); iter; iter = clib_hashmap_iter_next(&animator->animations, iter))
		AnimationDestroy(animation_hashmap_iter_get_value(iter)->animation);

	clib_hashmap_destroy(&animator->animations);
	clib_hashmap_destroy(&animator->conditions);
}

void AnimatorRegisterConition(Animator* animator, const char* name, int (*condition)(EcsEntity*, int))
{
	_conditionkvp* kvp = _condition_kvp(name, condition);

	if (kvp && condition_hashmap_insert(&animator->conditions, kvp->key, kvp) != kvp)
		free(kvp);
}

void AnimatorAddAnimation(Animator* animator, const char* name, Animation* animation)
{
	_animationkvp* kvp = _animation_kvp(name, animation);

	if (kvp)
	{
		if (animation_hashmap_insert(&animator->animations, kvp->key, kvp) != kvp)
		{
			AnimationDestroy(animation);
			free(animation);
			free(kvp);
			return;
		}

		if (animator->current == NULL)
			animator->current = kvp->key;
	}
}

void AnimatorTick(Animator* animator, EcsEntity* entity, float deltatime)
{
	if (!animator->current)
		return;

	_animationkvp* kvp = animation_hashmap_get(&animator->animations, animator->current);
	if (kvp)
	{
		AnimationTick(kvp->animation, deltatime);
		clib_strmap* transitions = &kvp->animation->transitions;
		for (clib_strmap_iter* iter = clib_strmap_iterator(transitions); iter; iter = clib_strmap_iter_next(transitions, iter))
		{
			_conditionkvp* cond = condition_hashmap_get(&animator->conditions, clib_strmap_iter_get_key(iter));
			if (cond && cond->condition(entity, 0))
				AnimatorPlay(animator, clib_strmap_iter_get_value(iter));
		}
	}
}

void AnimatorPlay(Animator* animator, const char* name)
{
	if (strcmp(animator->current, name) == 0)
		return;

	_animationkvp* kvp = animation_hashmap_get(&animator->animations, name);
	if (kvp)
	{
		AnimationStart(kvp->animation);
		animator->current = kvp->key;
	}
}

int AnimatorGetFrame(Animator* animator)
{
	if (!animator->current) return 0;

	_animationkvp* kvp = animation_hashmap_get(&animator->animations, animator->current);
	if (kvp)
		return kvp->animation->frame;
		
	return 0;
}

void AnimatorDebugPrint(Animator* animator)
{
	for (clib_hashmap_iter* iter = clib_hashmap_iterator(&animator->conditions); iter; iter = clib_hashmap_iter_next(&animator->conditions, iter))
		printf("Condition: %s\n", condition_hashmap_iter_get_value(iter)->key);

	for (clib_hashmap_iter* iter = clib_hashmap_iterator(&animator->animations); iter; iter = clib_hashmap_iter_next(&animator->animations, iter))
	{
		printf("Animation: %s\n", animation_hashmap_iter_get_value(iter)->key);

		Animation* animation = animation_hashmap_iter_get_value(iter)->animation;

		for (clib_hashmap_iter* a_iter = clib_hashmap_iterator(&animation->transitions); a_iter; a_iter = clib_hashmap_iter_next(&animation->transitions, a_iter))
			printf("Transition: %s - %s\n", clib_strmap_iter_get_key(a_iter), clib_strmap_iter_get_value(a_iter));
	}
}

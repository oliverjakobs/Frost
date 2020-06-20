#include "AnimationSystem.h"

static void PlayAnimation(Animator* animator, const char* name)
{
	if (strcmp(animator->current, name) == 0)
		return;

	Animation* animation = clib_dict_find(&animator->animations, name);
	if (animation)
	{
		AnimationStart(animation);
		animator->current = clib_dict_get_key_ptr(&animator->animations, name);
	}
}

void EcsSystemAnimation(ComponentTable* components, const char* entity, float deltatime)
{
	Animator* animator = ComponentTableGetComponent(components, entity, COMPONENT_ANIMATION);
	Sprite* sprite = ComponentTableGetComponent(components, entity, COMPONENT_TEXTURE);

	if (!(animator && sprite)) return;

	if (!animator->current)
	{
		sprite->frame = 0;
		return;
	}

	Animation* current = clib_dict_find(&animator->animations, animator->current);
	if (current)
	{
		AnimationTick(current, deltatime);
		clib_hashmap* transitions = &current->transitions;
		for (clib_hashmap_iter* iter = clib_strmap_iterator(transitions); iter; iter = clib_strmap_iter_next(transitions, iter))
		{
			AnimationCondition* cond = clib_dict_find(&animator->conditions, clib_strmap_iter_get_key(iter));
			if (cond && cond->func(components, entity, 0))
				PlayAnimation(animator, clib_strmap_iter_get_value(iter));
		}
		sprite->frame = current->frame;
	}
	else
	{
		sprite->frame = 0;
	}
}

#include "AnimationSystem.h"

#include "Animation/AnimationConditions.h"

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

void AnimationSystem(Ecs* ecs, ComponentTable* components, float deltatime)
{
	COMPONENT_MAP_ITERATE_FOR(clib_array_get(&components->table, COMPONENT_ANIMATION), iter)
	{
		Animator* animator = ComponentMapIterValue(iter);

		Sprite* sprite = ComponentTableGetDataComponent(components, ComponentMapIterKey(iter), COMPONENT_SPRITE);

		if (!sprite) continue;

		Animation* current = clib_hashmap_find(&animator->animations, animator->current);
		if (!current)
		{
			sprite->frame = 0;
			continue;
		}

		AnimationTick(current, deltatime);
		clib_hashmap* transitions = &current->transitions;

		CLIB_DICT_ITERATE_FOR(transitions, transition_iter)
		{
			AnimationCondition* cond = AnimationConditionsGetCondition(clib_strmap_iter_get_key(transition_iter));
			if (cond && cond->func(components, *(EntityID*)clib_hashmap_iter_get_key(iter), 0))
				PlayAnimation(animator, clib_strmap_iter_get_value(transition_iter));
		}
		sprite->frame = current->frame;
	}
}

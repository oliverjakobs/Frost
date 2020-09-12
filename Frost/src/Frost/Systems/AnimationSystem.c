#include "AnimationSystem.h"

#include "Frost/FrostEcs.h"
#include "Frost/AnimationConditions.h"

static void PlayAnimation(Animator* animator, const char* name)
{
	if (strcmp(animator->current, name) == 0)
		return;

	Animation* animation = tb_hashmap_find(&animator->animations, name);
	if (animation)
	{
		AnimationStart(animation);
		animator->current = tb_hashmap_get_key_ptr(&animator->animations, name);
	}
}

void AnimationSystem(Ecs* ecs, float deltatime)
{
	EcsComponentMap* map = EcsGetComponentMap(ecs, COMPONENT_ANIMATION);
	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		Animator* animator = EcsComponentMapIterValue(iter);

		Sprite* sprite = EcsGetDataComponent(ecs, EcsComponentMapIterKey(iter), COMPONENT_SPRITE);

		if (!sprite) continue;

		Animation* current = tb_hashmap_find(&animator->animations, animator->current);
		if (!current)
		{
			sprite->frame = 0;
			continue;
		}

		AnimationTick(current, deltatime);
		tb_hashmap* transitions = &current->transitions;
		for (tb_hashmap_iter* t_iter = tb_hashmap_iterator(transitions); t_iter; t_iter = tb_hashmap_iter_next(transitions, t_iter))
		{
			AnimationCondition* cond = AnimationConditionsGetCondition(tb_hashmap_iter_get_key(t_iter));
			if (cond && cond->func(ecs, EcsComponentMapIterKey(iter), 0))
				PlayAnimation(animator, tb_hashmap_iter_get_value(t_iter));
		}
		sprite->frame = current->frame;
	}
}

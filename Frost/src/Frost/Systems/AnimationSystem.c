#include "AnimationSystem.h"

#include "Frost/FrostEcs.h"

static void PlayAnimation(Animator* animator, EntityState state)
{
	if (state == ENTITY_STATE_NULL || animator->current == state) return;

	Animation* animation = &animator->animations[state];
	if (animation)
	{
		AnimatorStart(animator, animation);
		animator->current = state;
	}
}

void AnimationSystem(Ecs* ecs, float deltatime)
{
	EcsComponentMap* map = EcsGetComponentMap(ecs, COMPONENT_ANIMATOR);
	for (EcsComponentMapIter* iter = EcsComponentMapIterator(map); iter; iter = EcsComponentMapIterNext(map, iter))
	{
		Animator* animator = EcsComponentMapIterValue(iter);

		Sprite* sprite = EcsGetDataComponent(ecs, EcsComponentMapIterKey(iter), COMPONENT_SPRITE);

		if (!sprite) continue;

		Animation* current = &animator->animations[animator->current];
		if (!current)
		{
			sprite->frame = 0;
		}
		else
		{
			AnimatorTick(animator, current, deltatime);
			PlayAnimation(animator, EntityGetState(ecs, EcsComponentMapIterKey(iter)));
			sprite->frame = animator->frame;
		}
	}
}
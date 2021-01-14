#include "AnimationSystem.h"

#include "Frost/FrostEcs.h"

static void AnimatorTick(Animator* animator, Animation* animation, float deltatime)
{
	animator->clock += deltatime;

	// change frame
	if (animator->clock > animation->delay)
	{
		animator->clock = 0.0f;
		animator->frame++;
	}

	// restart animation
	if (animator->frame >= animation->start + animation->length || animator->frame < animation->start)
		AnimatorStart(animator, animation->start);
}

void AnimationSystem(Ecs* ecs, const Scene* scene, float deltatime)
{
	EcsMap* map = EcsGetComponentMap(ecs, COMPONENT_ANIMATOR);
	for (EcsMapIter* iter = EcsMapIterator(map); iter; iter = EcsMapIterNext(map, iter))
	{
		Animator* animator = EcsMapIterValue(iter);
		ECS_REQUIRE_MAP(Sprite, ecs, sprite, iter, COMPONENT_SPRITE);

		Animation* current = &animator->animations[animator->current];
		if (current)
		{
			EntityState state = EntityGetState(ecs, EcsMapIterKey(iter));
			if (animator->current == state)
			{
				AnimatorTick(animator, current, deltatime);
			}
			else
			{
				Animation* animation = &animator->animations[state];
				if (animation)
				{
					AnimatorStart(animator, animation->start);
					animator->current = state;
				}
			}

			sprite->frame = animator->frame;
		}
		else
		{
			sprite->frame = 0;
		}
	}
}
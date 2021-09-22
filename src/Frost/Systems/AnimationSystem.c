#include "AnimationSystem.h"

#include "Frost/Frost.h"

static void AnimatorTick(Animator* animator, float deltatime)
{
	animator->clock += deltatime;

	// change frame
	if (animator->clock > animator->current->delay)
	{
		animator->clock = 0.0f;
		animator->frame++;
	}

	// restart animation
	if (animator->frame >= animator->current->start + animator->current->length || animator->frame < animator->current->start)
		AnimatorStart(animator, animator->current);
}

void AnimationSystem(Ecs* ecs, const Scene* scene, float deltatime)
{
	EcsMap* map = EcsGetComponentMap(ecs, COMPONENT_ANIMATOR);
	for (EcsMapIter* iter = EcsMapIterator(map); iter; iter = EcsMapIterNext(map, iter))
	{
		Animator* animator = EcsMapIterValue(iter);
		ECS_REQUIRE_MAP(Sprite, ecs, sprite, iter, COMPONENT_SPRITE);

		if (animator->current)
		{
			EntityState state = EntityGetState(ecs, EcsMapIterKey(iter));
			if (animator->state == state)
			{
				AnimatorTick(animator, deltatime);
			}
			else
			{
				Animation* animation = &animator->animations[state];
				if (animation)
				{
					AnimatorStart(animator, animation->start);
					animator->state = state;
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
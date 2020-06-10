#include "AnimationSystem.h"

static void PlayAnimation(EcsAnimationComponent* comp, const char* name)
{
	if (strcmp(comp->current, name) == 0)
		return;

	Animation* animation = (Animation*)clib_dict_get(&comp->animations, name);
	if (animation)
	{
		AnimationStart(animation);
		comp->current = clib_dict_get_key_ptr(&comp->animations, name);
	}
}

void EcsSystemAnimation(ComponentTable* components, const char* entity, float deltatime)
{
	EcsAnimationComponent* animation = ComponentTableGetComponent(components, entity, COMPONENT_ANIMATION);
	EcsTextureComponent* texture = ComponentTableGetComponent(components, entity, COMPONENT_TEXTURE);

	if (!(animation && texture)) return;

	if (!animation->current)
	{
		texture->frame = 0;
		return;
	}

	Animation* current = (Animation*)clib_dict_get(&animation->animations, animation->current);
	if (current)
	{
		AnimationTick(current, deltatime);
		clib_strmap* transitions = &current->transitions;
		for (clib_strmap_iter* iter = clib_strmap_iterator(transitions); iter; iter = clib_strmap_iter_next(transitions, iter))
		{
			AnimationCondition* cond = (AnimationCondition*)clib_dict_get(&animation->conditions, clib_strmap_iter_get_key(iter));
			if (cond && cond->func(components, entity, 0))
				PlayAnimation(animation, clib_strmap_iter_get_value(iter));
		}
		texture->frame = current->frame;
	}
	else
	{
		texture->frame = 0;
	}
}

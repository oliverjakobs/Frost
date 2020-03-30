#include "Animator.hpp"

#include "AnimationConditions.h"

void AnimatorInit(Animator* animator)
{
	AnimatorRegisterConition(animator, "condition_jump", AnimationConditionJump);
	AnimatorRegisterConition(animator, "condition_fall", AnimationConditionFall);
	AnimatorRegisterConition(animator, "condition_walk", AnimationConditionWalk);
	AnimatorRegisterConition(animator, "condition_idle", AnimationConditionIdle);
}

void AnimatorDestroy(Animator* animator)
{
}

void AnimatorRegisterConition(Animator* animator, const std::string& name, std::function<bool(Entity*, int)> condition)
{
	animator->conditions.insert({ name, condition });
}

void AnimatorCreateAnimation(Animator* animator, const std::string& name, int start, int length, float delay, std::vector<Transition> transitions)
{
	auto animation = std::make_shared<Animation>();
	AnimationLoad(animation.get(), start, length, delay);

	for (auto& transition : transitions)
	{
		AnimationAddTransition(animation.get(), transition.first, transition.second);
	}

	animator->animations.insert({ name, animation });

	if (animator->current.empty())
		animator->current = name;
}

void AnimatorTick(Animator* animator, Entity* entity, float deltatime)
{
	if (animator->current.empty())
		return;

	auto& animation = animator->animations.at(animator->current);

	AnimationTick(animation.get(), deltatime);
	for (const auto& [name, next] : animation->transitions)
	{
		if (animator->conditions.find(name) != animator->conditions.end())
		{
			if (animator->conditions.at(name)(entity, 0))
				AnimatorPlay(animator, next);
		}
	}
}

void AnimatorPlay(Animator* animator, const std::string& name)
{
	if (animator->current.compare(name) == 0)
		return;

	if (animator->animations.find(name) != animator->animations.end())
	{
		AnimationStart(animator->animations[name].get());
		animator->current = name;
	}
}

int AnimatorGetFrame(Animator* animator)
{
	if (animator->current.empty())
		return 0;

	return animator->animations.at(animator->current)->frame;
}

#pragma once

#include "ECS/Components.h"

struct ImageRenderSystem
{
	static void Tick(entt::registry& registry);
};

struct AnimationSystem
{
	static void PlayAnimation(const std::string& anim, AnimationComponent& animComp);

	static void Tick(entt::registry& registry);
};
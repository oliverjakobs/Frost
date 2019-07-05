#pragma once

#include "ecsSystem.h"

struct ImageRenderSystem : ecsSystem
{
	void Tick(entt::registry& registry) override;
};

struct AnimationSystem : ecsSystem
{
	static void PlayAnimation(const std::string& anim, AnimationComponent& animComp);

	void Tick(entt::registry& registry) override;
};
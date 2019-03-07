#pragma once

#include "Components.h"

class ImageRenderSystem : public BaseECSSystem
{
public:
	ImageRenderSystem() : BaseECSSystem()
	{
		addComponentType(ImageComponent::ID);
		addComponentType(PositionComponent::ID);
	}

	void tick(BaseECSComponent** components, float deltaTime) override
	{
		ImageComponent* imgComp = (ImageComponent*)components[0];
		PositionComponent* posComp = (PositionComponent*)components[1];

		float x = posComp->position.x - (imgComp->image->getWidth() / 2.0f);
		float y = posComp->position.y;

		imgComp->image->renderF(x, y, imgComp->frame, Renderer::GetViewMat(), "shader");
	}
};

class AnimationSystem : public BaseECSSystem
{
public:
	AnimationSystem() : BaseECSSystem()
	{
		addComponentType(ImageComponent::ID);
		addComponentType(AnimationComponent::ID);
		addComponentType(MovementComponent::ID);
	}

	static void PlayAnimation(const std::string& anim, AnimationComponent* animComp)
	{
		if (stringCompare(animComp->currentAnimation, anim))
			return;

		auto pos = animComp->animations.find(anim);

		if (pos != animComp->animations.end())
		{
			animComp->animations[anim].start();
			animComp->currentAnimation = anim;
		}
	}

	void tick(BaseECSComponent** components, float deltaTime) override
	{
		ImageComponent* imgComp = (ImageComponent*)components[0];
		AnimationComponent* animComp = (AnimationComponent*)components[1];
		MovementComponent* moveComp = (MovementComponent*)components[2];

		if (!animComp->currentAnimation.empty())
		{
			animComp->animations[animComp->currentAnimation].step(deltaTime);
			imgComp->frame = animComp->animations[animComp->currentAnimation].getFrame();
		}
	}
};



#include "RenderSystem.h"

#include "String/StringUtils.h"

void ImageRenderSystem::Tick(entt::registry& registry)
{
	registry.view<ImageComponent, TransformComponent>().each([](auto entity, auto& img, auto& trans)
	{
		float x = trans.position.x - (img.image->getWidth() / 2.0f);
		float y = trans.position.y;
		img.image->renderF(x, y, img.frame, Renderer::GetViewMat(), "shader");
	});
};

void AnimationSystem::PlayAnimation(const std::string& anim, AnimationComponent& animComp)
{
	if (stringCompare(animComp.currentAnimation, anim))
		return;

	auto pos = animComp.animations.find(anim);

	if (pos != animComp.animations.end())
	{
		animComp.animations[anim].start();
		animComp.currentAnimation = anim;
	}
}

void AnimationSystem::Tick(entt::registry& registry)
{
	registry.view<ImageComponent, AnimationComponent>().each([](auto entity, auto& img, auto& anim)
	{
		if (!anim.currentAnimation.empty())
		{
			anim.animations[anim.currentAnimation].step(Timer::GetDeltaTime());
			img.frame = anim.animations[anim.currentAnimation].getFrame();
		}
	});
}

#include "AnimationComponent.h"

#include "Obelisk/Obelisk.h"

Animation::Animation(int start, int length, float delay)
	: Start(start), Length(length), Delay(delay), FrameCounter(0.0f), Frame(0)
{
}

static void StartAnimation(Animation& anim)
{
	anim.Frame = anim.Start;
	anim.FrameCounter = 0.0f;
}

static void StepAnimation(Animation& anim, float deltaTime)
{
	anim.FrameCounter += deltaTime;

	if (anim.FrameCounter > anim.Delay)
	{
		anim.FrameCounter = 0.0f;
		anim.Frame++;
	}

	if (anim.Frame >= anim.Start + anim.Length || anim.Frame < anim.Start)
	{
		anim.Frame = anim.Start;
		anim.FrameCounter = 0.0f;
	}
}

AnimationComponent::AnimationComponent(Entity* entity, const std::map<std::string, Animation>& anims)
	: Component(entity), m_animations(anims), m_currentAnimation("") 
{
}

void AnimationComponent::PlayAnimation(const std::string& anim)
{
	if (obelisk::StringCompare(m_currentAnimation, anim))
		return;

	if (m_animations.find(anim) != m_animations.end())
	{
		StartAnimation(m_animations[anim]);
		m_currentAnimation = anim;
	}
}

std::string AnimationComponent::GetCurrent() const
{
	return m_currentAnimation;
}

void AnimationComponent::OnUpdate(Scene* scene, float deltaTime)
{
	if (!m_currentAnimation.empty())
	{
		StepAnimation(m_animations[m_currentAnimation], deltaTime);

		TextureComponent* tex = m_entity->GetComponent<TextureComponent>();

		if (tex != nullptr)
			tex->SetFrame(m_animations[m_currentAnimation].Frame);
	}
}

void AnimationComponent::OnRender(Scene* scene)
{
}

#include "AnimationComponent.h"

#include "String/StringUtils.h"
#include "Utility/Timer.h"

AnimationComponent::AnimationComponent(std::map<std::string, Animation> anims)
	: m_animations(anims), m_currentAnimation("") 
{
}

void AnimationComponent::PlayAnimation(const std::string& anim)
{
	if (stringCompare(m_currentAnimation, anim))
		return;

	if (m_animations.find(anim) != m_animations.end())
	{
		m_animations[anim].start();
		m_currentAnimation = anim;
	}
}

std::string AnimationComponent::GetCurrent() const
{
	return m_currentAnimation;
}

void AnimationComponent::OnUpdate()
{
	if (!m_currentAnimation.empty())
	{
		m_animations[m_currentAnimation].step(Timer::GetDeltaTime());

		ImageComponent* img = m_entity->GetComponent<ImageComponent>();

		if (img != nullptr)
			img->SetFrame(m_animations[m_currentAnimation].getFrame());
	}
}

void AnimationComponent::OnRender()
{
}

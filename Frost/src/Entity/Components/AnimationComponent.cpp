#include "AnimationComponent.h"

#include "Obelisk/Obelisk.h"

AnimationComponent::AnimationComponent(Entity* entity, std::map<std::string, Animation> anims)
	: Component(entity), m_animations(anims), m_currentAnimation("") 
{
}

void AnimationComponent::PlayAnimation(const std::string& anim)
{
	if (obelisk::StringCompare(m_currentAnimation, anim))
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

void AnimationComponent::OnUpdate(float deltaTime)
{
	if (!m_currentAnimation.empty())
	{
		m_animations[m_currentAnimation].step(deltaTime);

		TextureComponent* tex = m_entity->GetComponent<TextureComponent>();

		if (tex != nullptr)
			tex->SetFrame(m_animations[m_currentAnimation].getFrame());
	}
}

void AnimationComponent::OnRender()
{
}

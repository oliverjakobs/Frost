#include "AnimationComponent.h"

#include "Obelisk/Obelisk.h"

Animation::Animation(int start, int length, float delay)
	: m_start(start), m_length(length), m_delay(delay), m_frameCounter(0.0f), m_frame(0)
{
}

void Animation::Start()
{
	m_frame = m_start;
	m_frameCounter = 0.0f;
}

void Animation::Step(float deltaTime)
{
	m_frameCounter += deltaTime;

	if (m_frameCounter > m_delay)
	{
		m_frameCounter = 0.0f;
		m_frame++;
	}

	if (m_frame >= m_start + m_length || m_frame < m_start)
	{
		m_frame = m_start;
		m_frameCounter = 0.0f;
	}
}

int Animation::GetFrame()
{
	return m_frame;
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
		m_animations[anim].Start();
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
		m_animations[m_currentAnimation].Step(deltaTime);

		TextureComponent* tex = m_entity->GetComponent<TextureComponent>();

		if (tex != nullptr)
			tex->SetFrame(m_animations[m_currentAnimation].GetFrame());
	}
}

void AnimationComponent::OnRender()
{
}

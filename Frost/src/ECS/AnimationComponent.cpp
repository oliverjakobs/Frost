#include "AnimationComponent.h"

#include "Scrapbook/Utility.h"

Animation::Animation(int start, int length, float delay)
	: m_start(start), m_length(length), m_delay(delay), m_frameCounter(0.0f)
{

}

Animation::~Animation()
{
}

void Animation::setStart(int s)
{
	m_start = s;

	start();
}

void Animation::start()
{
	m_frame = m_start;
	m_frameCounter = 0.0f;
}

void Animation::step(float deltaTime)
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

int Animation::getFrame()
{
	return m_frame;
}

AnimationComponent::AnimationComponent(Image* sprite, std::map<std::string, Animation*> animations)
{
	m_sprite = sprite;

	m_currentAnimation = nullptr;
	m_animations = animations;
}

AnimationComponent::~AnimationComponent()
{
}

void AnimationComponent::flip(RenderFlip flip)
{
	m_sprite->setRenderFlip(flip);
}

void AnimationComponent::play(const std::string anim)
{
	if (stringCompare(m_currentName, anim))
		return;

	m_currentAnimation = m_animations[anim];

	if (m_currentAnimation != nullptr)
	{
		m_currentAnimation->start();
		m_currentName = anim;
	}
}

bool AnimationComponent::setEntity(Entity* e)
{
	m_physComp = getComponent<PhysicsComponent>(e);

	if (m_physComp == nullptr)
	{
		DEBUG_MESSAGE("No Physics Component");
		return false;
	}

	m_entity = e;
	return m_entity != nullptr;
}

void AnimationComponent::onInput()
{
}

void AnimationComponent::onUpdate()
{
	if (m_currentAnimation != nullptr)
		m_currentAnimation->step(Timer::GetDeltaTime());
}

void AnimationComponent::onRender() const
{
	float x = m_entity->getPosition().x - m_sprite->getWidth() / 2.0f;
	float y = m_entity->getPosition().y;

	if (m_currentAnimation != nullptr)
		m_sprite->renderF(x, y, m_currentAnimation->getFrame(), sb::ResourceManager::GetShader("shader"));
}
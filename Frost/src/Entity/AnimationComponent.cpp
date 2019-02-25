#include "AnimationComponent.h"

#include "Scrapbook/Utility.h"

Animation::Animation()
	: m_start(0), m_length(0), m_delay(0.0f), m_frameCounter(0.0f)
{
}

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

AnimationComponent::AnimationComponent(const AnimationComponent& copy)
{
	m_sprite = copy.m_sprite;

	m_currentAnimation = nullptr;

	m_animations = copy.m_animations;
}

AnimationComponent::AnimationComponent(Image* sprite, std::map<std::string, Animation> animations)
{
	m_sprite = sprite;

	m_currentAnimation = nullptr;
	m_animations = animations;
}

AnimationComponent::~AnimationComponent()
{
	m_animations.clear();
	m_currentAnimation = nullptr;
}

AnimationComponent* AnimationComponent::clone()
{
	return new AnimationComponent(*this);
}

void AnimationComponent::flip(RenderFlip flip)
{
	m_sprite->setRenderFlip(flip);
}

void AnimationComponent::play(const std::string anim)
{
	if (stringCompare(m_currentName, anim))
		return;

	m_currentAnimation = std::make_unique<Animation>(m_animations[anim]);

	if (m_currentAnimation != nullptr)
	{
		m_currentAnimation->start();
		m_currentName = anim;
	}
}

bool AnimationComponent::setEntity(Entity* e)
{
	m_physComp = e->getComponent<PhysicsComponent>();

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
		m_sprite->renderF(x, y, m_currentAnimation->getFrame(), Renderer::GetViewMat(), "shader");
}
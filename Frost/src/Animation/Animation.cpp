#include "Animation.h"

#include "Entity/Entity.h"

Animation::Animation(int start, int length, float delay)
	: m_start(start), m_length(length), m_delay(delay), m_clock(0.0f), m_frame(0)
{
}

void Animation::Start()
{
	m_frame = m_start;
	m_clock = 0.0f;
}

void Animation::Tick(float deltaTime)
{
	m_clock += deltaTime;

	// change frame
	if (m_clock > m_delay)
	{
		m_clock = 0.0f;
		m_frame++;
	}

	// restart animation
	if (m_frame >= m_start + m_length || m_frame < m_start)
	{
		m_frame = m_start;
		m_clock = 0.0f;
	}
}

void Animation::AddContition(const std::string& name, const std::string& next)
{
	m_transitions.insert({ name, next });
}

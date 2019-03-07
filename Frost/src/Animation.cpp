#include "Animation.h"

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
#include "Timer.h"

namespace sb
{
	Timer::Timer()
	{
		m_timer = 0.0f;
		m_frames = 0;
		m_fps = 0;

		m_deltaTime = 0.0f;
		m_lastFrame = 0.0f;
	}

	Timer::~Timer()
	{
	}

	void Timer::start(float time)
	{
		float currentFrame = time;
		m_deltaTime = currentFrame - m_lastFrame;
		m_lastFrame = currentFrame;
	}

	void Timer::end(float time)
	{
		m_frames++;
		if (time - m_timer > 1.0f)
		{
			m_timer += 1.0f;
			m_fps = m_frames;
			m_frames = 0;
		}
	}

	float Timer::getDeltaTime() const
	{
		return m_deltaTime;
	}

	int Timer::getFPS() const
	{
		return m_fps;
	}
}

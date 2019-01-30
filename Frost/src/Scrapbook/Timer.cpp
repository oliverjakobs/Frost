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

	void Timer::Start(float time)
	{
		Get()->m_deltaTime = time - Get()->m_lastFrame;
		Get()->m_lastFrame = time;
	}

	void Timer::End(float time)
	{
		Get()->m_frames++;
		if (time - Get()->m_timer > 1.0f)
		{
			Get()->m_timer += 1.0f;
			Get()->m_fps = Get()->m_frames;
			Get()->m_frames = 0;
		}
	}

	float Timer::GetDeltaTime()
	{
		return Get()->m_deltaTime;
	}

	int Timer::GetFPS()
	{
		return Get()->m_fps;
	}
}

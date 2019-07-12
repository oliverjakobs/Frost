#include "Timer.h"

#include <GLFW/glfw3.h>
#include <algorithm>

void Timer::updateFPS()
{
	m_timer += 1.0f;
	m_fps = m_frames;
	m_frames = 0;
}

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

void Timer::Start()
{
	float time = GetTime();

	Get().m_deltaTime = time - Get().m_lastFrame;
	Get().m_lastFrame = time;
}

void Timer::End()
{
	Get().m_frames++;
	if (GetTime() - Get().m_timer > 1.0f)
	{
		Get().updateFPS();
	}
}

float Timer::GetDeltaTime()
{
	return Get().m_deltaTime;
}

int Timer::GetFPS()
{
	return Get().m_fps;
}

float Timer::GetTime()
{
	return (float)glfwGetTime();
}

float Timer::GetTimeMS()
{
	return GetTime() * 1000.0f;
}

SimulationTime::SimulationTime()
{
	m_average = 0.0f;
	m_max = 0.0f;
	m_min = 0.0f;

	m_runningAverage = 0.0f;
	m_count = 0;
}

void SimulationTime::SetTime(float time)
{
	m_max = std::max(m_max, time);
	m_min = std::min((m_min <= 0.0f) ? time : m_min, time);

	m_runningAverage += time;
	m_average = m_runningAverage / ++m_count;
}

float SimulationTime::GetAverage() const
{
	return m_average;
}

float SimulationTime::GetMax() const
{
	return m_max;
}

float SimulationTime::GetMin() const
{
	return m_min;
}

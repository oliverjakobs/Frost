#include "Timer.h"

#include <GLFW/glfw3.h>

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
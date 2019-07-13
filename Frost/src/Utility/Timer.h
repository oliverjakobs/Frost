#pragma once

#include "Singelton.h"

class Timer : private Singleton<Timer>
{
private:
	float m_timer;
	int m_frames;
	int m_fps;

	float m_deltaTime;
	float m_lastFrame;

	void updateFPS();
public:
	Timer();
	~Timer();

	static void Start();
	static void End();

	static float GetDeltaTime();
	static int GetFPS();

	static float GetTime();
	static float GetTimeMS();
};
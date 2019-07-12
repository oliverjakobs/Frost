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

class SimulationTime
{
private:
	float m_average;
	float m_max;
	float m_min;

	float m_runningAverage;
	unsigned int m_count;

public:
	SimulationTime();

	void SetTime(float time);

	float GetAverage() const;
	float GetMax() const;
	float GetMin() const;
};
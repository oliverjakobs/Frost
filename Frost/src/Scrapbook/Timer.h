#pragma once

#include "Util\Singelton.h"

namespace sb
{
	class Timer : private Singleton<Timer>
	{
	private:
		float m_timer;
		int m_frames;
		int m_fps;

		float m_deltaTime;
		float m_lastFrame;
	public:
		Timer();
		~Timer();

		static void Start(float time);
		static void End(float time);

		static float GetDeltaTime();
		static int GetFPS();
	};
}
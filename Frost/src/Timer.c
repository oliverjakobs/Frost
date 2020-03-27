#include "Timer.h"

void TimerReset(Timer* timer)
{
	timer->seconds = 0.0f;
	timer->frames = 0;
	timer->fps = 0;

	timer->deltatime = 0.0f;
	timer->lastframe = 0.0f;
}

void TimerStart(Timer* timer, float seconds)
{
	timer->deltatime = seconds - timer->lastframe;
	timer->lastframe = seconds;
}

void TimerEnd(Timer* timer, float seconds)
{
	timer->frames++;
	if ((seconds - timer->seconds) > 1.0f)
	{
		timer->seconds += 1.0f;
		timer->fps = timer->frames;
		timer->frames = 0;
	}
}
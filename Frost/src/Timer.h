#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct
	{
		float seconds;
		int frames;
		int fps;

		float deltatime;
		float lastframe;
	} Timer;

	void TimerReset(Timer* timer);
	void TimerStart(Timer* timer, float seconds);
	void TimerEnd(Timer* timer, float seconds);

#ifdef __cplusplus
}
#endif

#endif /* !TIMER_H */

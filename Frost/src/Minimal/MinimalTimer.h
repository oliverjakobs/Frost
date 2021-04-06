#ifndef MINIMAL_TIMER_H
#define MINIMAL_TIMER_H

typedef struct
{
	double seconds;
	int frames;
	int fps;

	double deltatime;
	double lastframe;
} MinimalTimer;

void MinimalTimerReset(MinimalTimer* timer);
void MinimalTimerStart(MinimalTimer* timer, double seconds);
void MinimalTimerEnd(MinimalTimer* timer, double seconds);

#endif /* !MINIMAL_TIMER_H */

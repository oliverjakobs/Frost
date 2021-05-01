 #include "EventHandler.h"

#include <stdlib.h>

typedef struct
{
	Event* queue;
	size_t queue_size;
	void (*callback)(MinimalApp*, Event);
} EventHandler;

static EventHandler event_handler;

static Event* _EventHandlerGetNext()
{
	for (size_t i = 0; i < event_handler.queue_size; ++i)
	{
		if (event_handler.queue[i].type == EVENT_UNKOWN)
			return &event_handler.queue[i];
	}

	/* Event queue is full */
	return NULL;
}

uint8_t EventHandlerInit(size_t queue_size, void (*callback)(MinimalApp*, Event))
{
	event_handler.queue = calloc(queue_size, sizeof(Event));

	if (!event_handler.queue) return 0;

	event_handler.queue_size = queue_size;
	event_handler.callback = callback;

	return 1;
}

void EventHandlerDestroy()
{
	free(event_handler.queue);
}

void EventHandlerThrowWindowEvent(EventType type, uint32_t width, uint32_t height)
{
	Event* e = _EventHandlerGetNext();
	if (!e) return;

	e->type = type;
	e->window.width = width;
	e->window.height = height;
}

void EventHandlerThrowKeyEvent(EventType type, int32_t keycode, uint32_t repeatcount)
{
	Event* e = _EventHandlerGetNext();
	if (!e) return;

	e->type = type;
	e->key.keycode = keycode;
	e->key.repeatcount = repeatcount;
}

void EventHandlerThrowMouseButtonEvent(EventType type, int32_t button)
{
	Event* e = _EventHandlerGetNext();
	if (!e) return;

	e->type = type;
	e->mousebutton.buttoncode = button;
}

void EventHandlerThrowMouseEvent(EventType type, int32_t x, int32_t y)
{
	Event* e = _EventHandlerGetNext();
	if (!e) return;

	e->type = type;
	e->mouse.x = x;
	e->mouse.y = y;
}

void EventHandlerPoll(MinimalApp* app)
{
	for (size_t i = 0; i < event_handler.queue_size; ++i)
	{
		switch (event_handler.queue[i].type)
		{
		case EVENT_UNKOWN:		 continue;
		case EVENT_WINDOW_CLOSE: MinimalClose(app); break;
		default:
			event_handler.callback(app, event_handler.queue[i]);
			EventReset(&event_handler.queue[i]);
			break;
		}
	}
}

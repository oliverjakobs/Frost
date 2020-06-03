 #include "EventHandler.h"

#include "Debugger.h"

typedef struct
{
	Event* queue;
	size_t queue_size;
	void (*callback)(Application*, Event);
} EventHandler;

static EventHandler event_handler;

static void _EventHandlerResetEvent(Event* e)
{
	e->type = EVENT_UNKOWN;
	e->handled = 0;
}

static Event* _EventHandlerGetNext()
{
	for (size_t i = 0; i < event_handler.queue_size; ++i)
	{
		if (event_handler.queue[i].type == EVENT_UNKOWN)
			return &event_handler.queue[i];
	}

	DEBUG_WARN("[EventHandler] Event queue overflow\n");

	return NULL;
}

int EventHandlerInit(size_t queue_size, void (*callback)(Application*, Event))
{
	event_handler.queue = (Event*)malloc(queue_size * sizeof(Event));
	event_handler.queue_size = queue_size;
	event_handler.callback = callback;

	for (size_t i = 0; i < queue_size; ++i)
	{
		_EventHandlerResetEvent(&event_handler.queue[i]);
	}

	return 0;
}

void EventHandlerDestroy()
{
	free(event_handler.queue);
}

void EventHandlerThrowWindowEvent(EventType type, int width, int height)
{
	Event* e = _EventHandlerGetNext();

	if (e)
	{
		e->type = type;
		e->window.width = width;
		e->window.height = height;
	}
}

void EventHandlerThrowKeyEvent(EventType type, int keycode, int repeatcount)
{
	Event* e = _EventHandlerGetNext();

	if (e)
	{
		e->type = type;
		e->key.keycode = keycode;
		e->key.repeatcount = repeatcount;
	}
}

void EventHandlerThrowMouseButtonEvent(EventType type, int button)
{
	Event* e = _EventHandlerGetNext();

	if (e)
	{
		e->type = type;
		e->mousebutton.buttoncode = button;
	}
}

void EventHandlerThrowMouseEvent(EventType type, float x, float y)
{
	Event* e = _EventHandlerGetNext();

	if (e)
	{
		e->type = type;
		e->mouse.x = x;
		e->mouse.y = y;
	}
}

void EventHandlerThrowConsoleEvent(EventType type, const char* cmd)
{
	Event* e = _EventHandlerGetNext();

	if (e)
	{
		e->type = type;
		e->console.cmd = cmd;
	}
}

void EventHandlerPoll(Application* app)
{
	for (size_t i = 0; i < event_handler.queue_size; ++i)
	{
		if (event_handler.queue[i].type != EVENT_UNKOWN)
		{
			event_handler.callback(app, event_handler.queue[i]);
			_EventHandlerResetEvent(&event_handler.queue[i]);
		}
	}
}

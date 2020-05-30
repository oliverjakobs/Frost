#include "EventHandler.h"

#include "clib/vector.h"

CLIB_VECTOR_DEFINE_FUNCS(event_queue, Event)

typedef struct
{
	clib_vector queue;
	void (*callback)(Application*, const Event);
} EventHandler;

static EventHandler event_handler;

int EventHandlerInit()
{
	clib_vector_init(&event_handler.queue, EVENT_HANDLER_INITIAL_QUEUE_SIZE);

	return 0;
}

void EventHandlerDestroy()
{
	for (size_t i = 0; i < event_handler.queue.size; i++)
		free(event_queue_vector_get(&event_handler.queue, i));
	clib_vector_free(&event_handler.queue);
}

void EventHandlerSetEventCallback(void (*callback)(Application*, const Event))
{
	event_handler.callback = callback;
}

static Event* _EventHandlerAllocate(EventType type)
{
	Event* e = (Event*)malloc(sizeof(Event));

	if (e)
	{
		e->type = type;
		e->handled = 0;
	}

	return e;
}

void EventHandlerThrowWindowEvent(EventType type, int width, int height)
{
	Event* e = _EventHandlerAllocate(type);

	if (e)
	{
		e->window.width = width;
		e->window.height = height;
		event_queue_vector_push(&event_handler.queue, e);
	}
}

void EventHandlerThrowKeyEvent(EventType type, int keycode, int repeatcount)
{
	Event* e = _EventHandlerAllocate(type);

	if (e)
	{
		e->key.keycode = keycode;
		e->key.repeatcount = repeatcount;
		event_queue_vector_push(&event_handler.queue, e);
	}
}

void EventHandlerThrowMouseButtonEvent(EventType type, int button)
{
	Event* e = _EventHandlerAllocate(type);

	if (e)
	{
		e->mousebutton.buttoncode = button;
		event_queue_vector_push(&event_handler.queue, e);
	}
}

void EventHandlerThrowMouseEvent(EventType type, float x, float y)
{
	Event* e = _EventHandlerAllocate(type);

	if (e)
	{
		e->mouse.x = x;
		e->mouse.y = y;
		event_queue_vector_push(&event_handler.queue, e);
	}
}

void EventHandlerThrowConsoleEvent(EventType type, const char* cmd)
{
	Event* e = _EventHandlerAllocate(type);

	if (e)
	{
		e->console.cmd = cmd;
		event_queue_vector_push(&event_handler.queue, e);
	}
}

void EventHandlerPoll(Application* app)
{
	while (event_handler.queue.size > 0)
	{
		Event* e = event_queue_vector_get(&event_handler.queue, 0);

		event_handler.callback(app, *e);

		event_queue_vector_delete(&event_handler.queue, 0);

		free(e);
	}
}
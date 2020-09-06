#ifndef EVENT_H
#define EVENT_H

#include <stddef.h>
#include <stdint.h>

typedef enum
{
	EVENT_UNKOWN = 0,
	/* window events*/
	EVENT_WINDOW_CLOSE,
	EVENT_WINDOW_RESIZE,
	EVENT_WINDOW_FOCUS,
	EVENT_WINDOW_LOST_FOCUS,
	EVENT_WINDOW_MOVED,
	/* key events */
	EVENT_KEY_PRESSED,
	EVENT_KEY_RELEASED,
	EVENT_KEY_TYPED,
	/* mouse events */
	EVENT_MOUSE_BUTTON_PRESSED,
	EVENT_MOUSE_BUTTON_RELEASED,
	EVENT_MOUSE_MOVED,
	EVENT_MOUSE_SCROLLED,
	/* console events */
	EVENT_CONSOLE_EXEC
} EventType;

typedef struct
{
	int32_t keycode;
	uint32_t repeatcount;
} KeyEvent;

typedef struct
{
	float x, y;
} MouseEvent;

typedef struct
{
	int32_t buttoncode;
} MouseButtonEvent;

typedef struct
{
	uint32_t width, height;
} WindowEvent;

typedef struct
{
	const char* cmd;
} ConsoleEvent;

typedef struct
{
	EventType type;
	union
	{
		KeyEvent key;
		MouseEvent mouse;
		MouseButtonEvent mousebutton;
		WindowEvent window;
		ConsoleEvent console;
	};
	uint8_t handled;
} Event;

void EventReset(Event* e);

/* Utility */
uint8_t EventCheckType(const Event* e, EventType type);

int32_t EventMouseButton(const Event* e);
int32_t EventMouseButtonPressed(const Event* e);
int32_t EventMouseButtonReleased(const Event* e);

int32_t EventKey(const Event* e);
int32_t EventKeyPressed(const Event* e);
int32_t EventKeyReleased(const Event* e);

char EventKeyTyped(const Event* e);

#endif // !EVENT_H
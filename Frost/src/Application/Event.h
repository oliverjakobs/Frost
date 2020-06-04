#ifndef EVENT_H
#define EVENT_H

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
	int keycode;
	int repeatcount;
} KeyEvent;

typedef struct
{
	float x, y;
} MouseEvent;

typedef struct
{
	int buttoncode;
} MouseButtonEvent;

typedef struct
{
	unsigned int width, height;
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
	int handled;
} Event;

/* Utility */
int EventMouseButton(Event* e);
int EventMouseButtonPressed(Event* e);
int EventMouseButtonReleased(Event* e);

int EventKey(Event* e);
int EventKeyPressed(Event* e);
int EventKeyReleased(Event* e);

char EventKeyTyped(Event* e);

#endif // !EVENT_H
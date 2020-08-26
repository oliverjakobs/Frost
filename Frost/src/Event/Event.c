#include "Event.h"

#include "Application/Input.h"

void EventReset(Event* e)
{
	e->type = EVENT_UNKOWN;
	e->handled = 0;
}

int EventCheckType(const Event* e, EventType type)
{
	return !e->handled && e->type == type;
}

int EventMouseButton(const Event* e)
{
	if (e->handled) return MOUSE_BUTTON_UNKNOWN;

	if (e->type == EVENT_MOUSE_BUTTON_PRESSED || e->type == EVENT_MOUSE_BUTTON_RELEASED)
		return e->mousebutton.buttoncode;

	return MOUSE_BUTTON_UNKNOWN;
}

int EventMouseButtonPressed(const Event* e)
{
	if (EventCheckType(e, EVENT_MOUSE_BUTTON_PRESSED)) return e->mousebutton.buttoncode;

	return MOUSE_BUTTON_UNKNOWN;
}

int EventMouseButtonReleased(const Event* e)
{
	if (EventCheckType(e, EVENT_MOUSE_BUTTON_RELEASED)) return e->mousebutton.buttoncode;

	return MOUSE_BUTTON_UNKNOWN;
}

int EventKey(const Event* e)
{
	if (e->handled) return KEY_UNKNOWN;

	if (e->type == EVENT_KEY_PRESSED || e->type == EVENT_KEY_RELEASED || e->type == EVENT_KEY_TYPED)
		return e->key.keycode;

	return KEY_UNKNOWN;
}

int EventKeyPressed(const Event* e)
{
	if (EventCheckType(e, EVENT_KEY_PRESSED)) return e->key.keycode;

	return KEY_UNKNOWN;
}

int EventKeyReleased(const Event* e)
{
	if (EventCheckType(e, EVENT_KEY_RELEASED)) return e->key.keycode;

	return KEY_UNKNOWN;
}

char EventKeyTyped(const Event* e)
{
	if (EventCheckType(e, EVENT_KEY_TYPED)) return (char)e->key.keycode;

	return '\0';
}

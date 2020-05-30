#include "Event.h"

int EventMouseButton(Event* e)
{
	if (e->type == EVENT_MOUSE_BUTTON_PRESSED ||
		e->type == EVENT_MOUSE_BUTTON_RELEASED)
		return e->mousebutton.buttoncode;

	return -1;
}

int EventMouseButtonPressed(Event* e)
{
	if (e->type == EVENT_MOUSE_BUTTON_PRESSED)
		return e->mousebutton.buttoncode;

	return -1;
}

int EventMouseButtonReleased(Event* e)
{
	if (e->type == EVENT_MOUSE_BUTTON_RELEASED)
		return e->mousebutton.buttoncode;

	return -1;
}

int EventKey(Event* e)
{
	if (e->type == EVENT_KEY_PRESSED ||
		e->type == EVENT_KEY_RELEASED ||
		e->type == EVENT_KEY_TYPED)
		return e->key.keycode;

	return -1;
}

int EventKeyPressed(Event* e)
{
	if (e->type == EVENT_KEY_PRESSED)
		return e->key.keycode;

	return -1;
}

int EventKeyReleased(Event* e)
{
	if (e->type == EVENT_KEY_RELEASED)
		return e->key.keycode;

	return -1;
}

char EventKeyTyped(Event* e)
{
	if (e->type == EVENT_KEY_TYPED)
		return (char)e->key.keycode;

	return '\0';
}

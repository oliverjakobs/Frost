#include "Event.h"

int EventMouseButtonPressed(Event* e, int buttoncode)
{
	return e->type == EVENT_MOUSE_BUTTON_PRESSED && e->mousebutton.buttoncode == buttoncode;
}

int EventMouseButtonReleased(Event* e, int buttoncode)
{
	return e->type == EVENT_MOUSE_BUTTON_RELEASED && e->mousebutton.buttoncode == buttoncode;
}

#include "EventCallback.h"

#include "EventHandler.h"

void MinimalEventCallback(MinimalApp* app, Event e)
{
	switch (e.type)
	{
	case EVENT_WINDOW_CLOSE:	MinimalClose(app); break;
	// default:					app->on_event(app, e); break;
	}
}

void MinimalWindowSizeCallback(MinimalWindow* window, uint32_t width, uint32_t height)
{
	EventHandlerThrowWindowEvent(EVENT_WINDOW_RESIZE, width, height);
}

void MinimalWindowCloseCallback(MinimalWindow* window)
{
	EventHandlerThrowWindowEvent(EVENT_WINDOW_CLOSE, 0, 0);
}

void MinimalKeyCallback(MinimalWindow* window, UINT keycode, UINT scancode, UINT action, UINT mods)
{
	switch (action)
	{
	case MINIMAL_PRESS:		EventHandlerThrowKeyEvent(EVENT_KEY_PRESSED, keycode, 0); break;
	case MINIMAL_RELEASE:	EventHandlerThrowKeyEvent(EVENT_KEY_RELEASED, keycode, 0); break;
	}
}

void MinimalCharCallback(MinimalWindow* window, UINT codepoint, UINT mods)
{
	EventHandlerThrowKeyEvent(EVENT_KEY_TYPED, codepoint, 0);
}

void MinimalMouseButtonCallback(MinimalWindow* window, UINT button, UINT action, UINT mods)
{
	switch (action)
	{
	case MINIMAL_PRESS:		EventHandlerThrowMouseButtonEvent(EVENT_MOUSE_BUTTON_PRESSED, button); break;
	case MINIMAL_RELEASE:	EventHandlerThrowMouseButtonEvent(EVENT_MOUSE_BUTTON_RELEASED, button); break;
	}
}

void MinimalScrollCallback(MinimalWindow* window, INT xOffset, INT yOffset)
{
	EventHandlerThrowMouseEvent(EVENT_MOUSE_SCROLLED, xOffset, yOffset);
}

void MinimalCursorPosCallback(MinimalWindow* window, INT xPos, INT yPos)
{
	MINIMAL_INFO("%d, %d", xPos, yPos);
	EventHandlerThrowMouseEvent(EVENT_MOUSE_MOVED, xPos, yPos);
}

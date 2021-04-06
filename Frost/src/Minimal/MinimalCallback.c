#include "MinimalCallback.h"

#include "Minimal.h"
#include "MinimalLogger.h"

void MinimalEventCallback(MinimalApp* app, Event e)
{
	switch (e.type)
	{
	case EVENT_WINDOW_CLOSE:	app->running = 0; break;
	default:					app->on_event(app, e); break;
	}
}

void MinimalGLFWErrorCallback(int error, const char* desc)
{
	MINIMAL_ERROR("[GLFW] (%d) %s", error, desc);
}

void MinimalGLFWWindowSizeCallback(GLFWwindow* window, int width, int height)
{
	MinimalApp* app = (MinimalApp*)glfwGetWindowUserPointer(window);

	app->width = width;
	app->height = height;

	EventHandlerThrowWindowEvent(EVENT_WINDOW_RESIZE, width, height);
}

void MinimalGLFWWindowCloseCallback(GLFWwindow* window)
{
	EventHandlerThrowWindowEvent(EVENT_WINDOW_CLOSE, 0, 0);
}

void MinimalGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:	EventHandlerThrowKeyEvent(EVENT_KEY_PRESSED, key, 0); break;
	case GLFW_RELEASE:	EventHandlerThrowKeyEvent(EVENT_KEY_RELEASED, key, 0); break;
	case GLFW_REPEAT:	EventHandlerThrowKeyEvent(EVENT_KEY_PRESSED, key, 1); break;
	}
}

void MinimalGLFWCharCallback(GLFWwindow* window, unsigned int keycode)
{
	EventHandlerThrowKeyEvent(EVENT_KEY_TYPED, keycode, 0);
}

void MinimalGLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:	EventHandlerThrowMouseButtonEvent(EVENT_MOUSE_BUTTON_PRESSED, button); break;
	case GLFW_RELEASE:	EventHandlerThrowMouseButtonEvent(EVENT_MOUSE_BUTTON_RELEASED, button); break;
	}
}

void MinimalGLFWScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	EventHandlerThrowMouseEvent(EVENT_MOUSE_SCROLLED, (float)xOffset, (float)yOffset);
}

void MinimalGLFWCursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	EventHandlerThrowMouseEvent(EVENT_MOUSE_MOVED, (float)xPos, (float)yPos);
}

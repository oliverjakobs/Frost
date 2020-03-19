#pragma once

#include "Event.hpp"

struct WindowResizeEvent : public Event
{
	unsigned int Width, Height;

	WindowResizeEvent(unsigned int width, unsigned int height) : Event(EventType::WindowResize), Width(width), Height(height) {}
};

struct WindowCloseEvent : public Event
{
	WindowCloseEvent() : Event(EventType::WindowClose) {}
};
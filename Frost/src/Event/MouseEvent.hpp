#pragma once

#include "Event.hpp"

struct MouseMovedEvent : public Event
{
	float MouseX, MouseY;

	MouseMovedEvent(float x, float y) : Event(EventType::MouseMoved), MouseX(x), MouseY(y) {}
};

struct MouseScrolledEvent : public Event
{
	float OffsetX, OffsetY;

	MouseScrolledEvent(float xOffset, float yOffset) : Event(EventType::MouseScrolled), OffsetX(xOffset), OffsetY(yOffset) {}
};

struct MouseButtonEvent : public Event
{
	int Button;

	MouseButtonEvent(EventType type, int button) : Event(type), Button(button) {}
};

struct MouseButtonPressedEvent : public MouseButtonEvent
{
	MouseButtonPressedEvent(int button) : MouseButtonEvent(EventType::MouseButtonPressed, button) {}
};

struct MouseButtonReleasedEvent : public MouseButtonEvent
{
	MouseButtonReleasedEvent(int button) : MouseButtonEvent(EventType::MouseButtonReleased, button) {}
};
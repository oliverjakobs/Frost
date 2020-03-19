#pragma once

enum class EventType
{
	None = 0,
	WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
	ChangeScene,
	KeyPressed, KeyReleased, KeyTyped,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

struct Event
{
	EventType Type;
	bool Handled;

	Event(EventType type) : Type(type), Handled(false) {}
};
#pragma once

#include "Event.hpp"

struct KeyEvent : public Event
{
	int KeyCode;

	KeyEvent(EventType type, int keycode) : Event(type), KeyCode(keycode) {}
};

struct KeyPressedEvent : public KeyEvent
{
	int RepeatCount;

	KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(EventType::KeyPressed, keycode), RepeatCount(repeatCount) {}
};

struct KeyReleasedEvent : public KeyEvent
{
	KeyReleasedEvent(int keycode) : KeyEvent(EventType::KeyReleased, keycode) {}
};

struct KeyTypedEvent : public KeyEvent
{
	KeyTypedEvent(int keycode) : KeyEvent(EventType::KeyTyped, keycode) {}
};
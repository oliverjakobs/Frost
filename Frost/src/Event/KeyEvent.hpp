#pragma once

#include "Event.hpp"

class KeyEvent : public Event
{
protected:
	int m_keyCode;

	KeyEvent(int keycode) : m_keyCode(keycode) {}
public:
	inline int GetKeyCode() const { return m_keyCode; }
};

class KeyPressedEvent : public KeyEvent
{
private:
	int m_repeatCount;

public:
	KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), m_repeatCount(repeatCount) {}

	inline int GetRepeatCount() const { return m_repeatCount; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << m_keyCode << " (" << m_repeatCount << " repeats)";
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyPressed);
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << m_keyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyReleased);
};

class KeyTypedEvent : public KeyEvent
{
public:
	KeyTypedEvent(int keycode) : KeyEvent(keycode) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << m_keyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyTyped);
};
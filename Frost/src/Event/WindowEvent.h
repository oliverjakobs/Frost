#pragma once

#include "Event.h"

class WindowResizeEvent : public Event
{
private:
	unsigned int m_width, m_height;

public:
	WindowResizeEvent(unsigned int width, unsigned int height) : m_width(width), m_height(height) {}

	inline unsigned int GetWidth() const { return m_width; }
	inline unsigned int GetHeight() const { return m_height; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << m_width << ", " << m_height;
		return ss.str();
	}

	EVENT_CLASS_TYPE(WindowResize);
};

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() {}

	EVENT_CLASS_TYPE(WindowClose);
};
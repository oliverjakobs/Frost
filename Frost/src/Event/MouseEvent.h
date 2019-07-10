#pragma once

#include "Event.h"

class MouseMovedEvent : public Event
{
private:
	float m_mouseX, m_mouseY;

public:
	MouseMovedEvent(float x, float y) : m_mouseX(x), m_mouseY(y) {}

	inline float GetX() const { return m_mouseX; }
	inline float GetY() const { return m_mouseY; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << m_mouseX << ", " << m_mouseY;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseMoved);
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);
};

class MouseScrolledEvent : public Event
{
private:
	float m_offsetX, m_offsetY;

public:
	MouseScrolledEvent(float xOffset, float yOffset) : m_offsetX(xOffset), m_offsetY(yOffset) {}

	inline float GetXOffset() const { return m_offsetX; }
	inline float GetYOffset() const { return m_offsetY; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << m_offsetX << ", " << m_offsetY;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseScrolled);
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);
};

class MouseButtonEvent : public Event
{
protected:
	int m_button;

	MouseButtonEvent(int button) : m_button(button) {}
public:
	inline int GetMouseButton() const { return m_button; }
	inline bool Is(unsigned int button) const { return m_button == button; }

	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << m_button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonPressed);
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << m_button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonReleased);
};
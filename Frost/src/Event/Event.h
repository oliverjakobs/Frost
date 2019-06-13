#pragma once

#include <string>
#include <sstream>

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

enum class EventType
{
	None = 0,
	WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
	AppTick, AppUpdate, AppRender,
	KeyPressed, KeyReleased, KeyTyped,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory
{
	None = 0,
	EventCategoryApplication =	BIT(0),
	EventCategoryInput =		BIT(1),
	EventCategoryKeyboard =		BIT(2),
	EventCategoryMouse =		BIT(3),
	EventCategoryMouseButton =	BIT(4)
};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

class Event
{
public:
	bool handled = false;

	virtual EventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual int GetCategoryFlags() const = 0;
	virtual std::string ToString() const { return GetName(); }

	inline bool IsInCategory(EventCategory category)
	{
		return GetCategoryFlags() & category;
	}
};

class EventDispatcher
{
private:
	Event& m_event;

	template<typename T>
	using EventCallback = std::function<bool(T&)>;
public:
	EventDispatcher(Event& event)
		: m_event(event)
	{
	}

	template<typename Type>
	bool Dispatch(EventCallback<Type> func)
	{
		if (m_event.GetEventType() == Type::GetStaticType())
		{
			m_event.handled = func(*(Type*)&m_event);
			return true;
		}
		return false;
	}
};

inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
	return os << e.ToString();
}
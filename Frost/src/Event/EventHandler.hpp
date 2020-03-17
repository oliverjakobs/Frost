#pragma once

#include "Obelisk/Obelisk.hpp"
#include "Obelisk/Singelton.hpp"

#include "Event/ApplicationEvent.hpp"
#include "Event/WindowEvent.hpp"
#include "Event/MouseEvent.hpp"
#include "Event/KeyEvent.hpp"

#include <queue>

using EventCallbackFunc = std::function<void(Event&)>;

class EventHandler : private Singleton<EventHandler>
{
private:
	std::queue<Event*> m_eventQueue;
	EventCallbackFunc m_eventCallback;

public:
	inline static void SetEventCallback(const EventCallbackFunc& callback)
	{
		Get().m_eventCallback = callback;
	}

	template<class EventType, typename... Args>
	inline static void Throw(Args... args)
	{
		Get().m_eventQueue.push(new EventType(std::forward<Args>(args)...));
	}

	inline static void Poll() 
	{
		while (!Get().m_eventQueue.empty())
		{
			Event* e = Get().m_eventQueue.front();

			Get().m_eventCallback(*e);
			Get().m_eventQueue.pop();

			SAFE_DELETE(e);
		}
	}
};
#pragma once

#include "Obelisk/Obelisk.hpp"
#include "Obelisk/Singelton.hpp"

#include "Event/ApplicationEvent.hpp"
#include "Event/WindowEvent.hpp"
#include "Event/MouseEvent.hpp"
#include "Event/KeyEvent.hpp"

#include <queue>

struct Application;

using EventCallbackFunc = std::function<void(Application*, Event&)>;

class EventHandler : private Singleton<EventHandler>
{
private:
	std::queue<Event*> m_eventQueue;
	EventCallbackFunc m_eventCallback;

	Application* app;

public:
	static void SetEventCallback(const EventCallbackFunc& callback);

	template<class EventType, typename... Args>
	inline static void Throw(Args... args)
	{
		Get().m_eventQueue.push(new EventType(std::forward<Args>(args)...));
	}

	static void Poll(Application* app);
};
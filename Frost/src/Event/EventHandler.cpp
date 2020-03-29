#include "EventHandler.hpp"

#include "Application.hpp"

void EventHandler::SetEventCallback(const EventCallbackFunc& callback)
{
	Get().m_eventCallback = callback;
}

void EventHandler::Poll(Application* app)
{
	while (!Get().m_eventQueue.empty())
	{
		Event* e = Get().m_eventQueue.front();

		Get().m_eventCallback(app, *e);
		Get().m_eventQueue.pop();

		SAFE_DELETE(e);
	}
}

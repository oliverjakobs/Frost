#include "EventHandler.h"

void EventHandler::SetEventCallback(const EventCallbackFunc& callback)
{
	Get().m_eventCallback = callback;
}

void EventHandler::ThrowEvent(Event& e)
{
	Get().m_eventCallback(e);
}

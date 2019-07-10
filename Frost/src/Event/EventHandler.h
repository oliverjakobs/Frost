#pragma once

#include "Utility/Singelton.h"

#include "Event/ApplicationEvent.h"
#include "Event/WindowEvent.h"
#include "Event/MouseEvent.h"
#include "Event/KeyEvent.h"

using EventCallbackFunc = std::function<void(Event&)>;

class EventHandler : private Singleton<EventHandler>
{
private:
	EventCallbackFunc m_eventCallback;

public:
	static void SetEventCallback(const EventCallbackFunc& callback);
	static void ThrowEvent(Event& e);
};
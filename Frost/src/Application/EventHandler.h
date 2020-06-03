#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Event.h"
#include "Application/Application.h"

int EventHandlerInit(size_t queue_size, void (*callback)(Application*, Event));
void EventHandlerDestroy();

void EventHandlerThrowWindowEvent(EventType type, int width, int height);
void EventHandlerThrowKeyEvent(EventType type, int keycode, int repeatcount);
void EventHandlerThrowMouseButtonEvent(EventType type, int button);
void EventHandlerThrowMouseEvent(EventType type, float x, float y);
void EventHandlerThrowConsoleEvent(EventType type, const char* cmd);

void EventHandlerPoll(Application* app);

#ifdef __cplusplus
}
#endif

#endif /* !EVENT_HANDLER_H */
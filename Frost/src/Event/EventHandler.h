#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "Event.h"

int EventHandlerInit(size_t queue_size, void (*callback)(void*, Event));
void EventHandlerDestroy();

void EventHandlerThrowWindowEvent(EventType type, int width, int height);
void EventHandlerThrowKeyEvent(EventType type, int keycode, int repeatcount);
void EventHandlerThrowMouseButtonEvent(EventType type, int button);
void EventHandlerThrowMouseEvent(EventType type, float x, float y);
void EventHandlerThrowConsoleEvent(EventType type, const char* cmd);

void EventHandlerPoll(void* context);

#endif /* !EVENT_HANDLER_H */
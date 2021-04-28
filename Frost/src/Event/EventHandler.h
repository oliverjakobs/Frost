#ifndef MINIMAL_EVENT_HANDLER_H
#define MINIMAL_EVENT_HANDLER_H

#include "Event.h"
#include "Minimal/Minimal.h"

uint8_t EventHandlerInit(size_t queue_size, void (*callback)(MinimalApp*, Event));
void EventHandlerDestroy();

void EventHandlerThrowWindowEvent(EventType type, uint32_t width, uint32_t height);
void EventHandlerThrowKeyEvent(EventType type, int32_t keycode, uint32_t repeatcount);
void EventHandlerThrowMouseButtonEvent(EventType type, int32_t button);
void EventHandlerThrowMouseEvent(EventType type, float x, float y);
void EventHandlerThrowConsoleEvent(EventType type, const char* cmd);

void EventHandlerPoll(MinimalApp* app);

#endif /* !MINIMAL_EVENT_HANDLER_H */
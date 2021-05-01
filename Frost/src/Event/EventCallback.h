#ifndef MINIMAL_CALLBACK_H
#define MINIMAL_CALLBACK_H

#include "Event.h"
#include "Minimal/Minimal.h"

/*
 * Gets called when an event is processed, handles some (e.g. WindowCloseEvents) 
 * and passes the rest to the on_event callback of app
 */
void MinimalEventCallback(MinimalApp* app, Event e);

/* Translates GLFW events to internal events */
void MinimalWindowSizeCallback  (MinimalWindow* wnd, uint32_t w, uint32_t h);
void MinimalWindowCloseCallback (MinimalWindow* wnd);
void MinimalKeyCallback         (MinimalWindow* wnd, UINT keycode, UINT scancode, UINT action, UINT mods);
void MinimalCharCallback        (MinimalWindow* wnd, UINT codepoint, UINT mods);
void MinimalMouseButtonCallback (MinimalWindow* wnd, UINT button, UINT action, UINT mods);
void MinimalScrollCallback      (MinimalWindow* wnd, INT x_offset, INT y_offset);
void MinimalCursorPosCallback   (MinimalWindow* wnd, INT x_pos, INT y_pos);

#endif /* !MINIMAL_CALLBACK_H */

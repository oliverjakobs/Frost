#ifndef MINIMAL_APPLICATION_H
#define MINIMAL_APPLICATION_H

#include <GLFW/glfw3.h>

#include "Utils.h"
#include "Input.h"
#include "Event.h"

/* --------------------------| minimal app |----------------------------- */
typedef int  (*MinimalLoadCB)    (MinimalApp* app, uint32_t w, uint32_t h);
typedef void (*MinimalDestroyCB) (MinimalApp* app);

typedef int  (*MinimalEventCB)   (MinimalApp* app, const MinimalEvent* e);
typedef void (*MinimalUpdateCB)  (MinimalApp* app, float deltatime);
typedef void (*MinimalRenderCB)  (MinimalApp* app);

struct MinimalApp {
    GLFWwindow* window;

    MinimalLoadCB    on_load;
    MinimalDestroyCB on_destroy;

    MinimalEventCB  on_event;
    MinimalUpdateCB on_update;
    MinimalRenderCB on_render;
    MinimalRenderCB on_render_debug;
    MinimalRenderCB on_render_ui;

    int debug;
    int vsync;

    MinimalTimer timer;
};

int MinimalLoad(MinimalApp* app, const char* title, uint32_t w, uint32_t h, const char* gl);
void MinimalDestroy(MinimalApp* app);

/* --------------------------| game loop |------------------------------- */
void MinimalRun(MinimalApp* app);
void MinimalClose(MinimalApp* app);

MinimalLoadCB    MinimalSetLoadCallback(MinimalApp* app, MinimalLoadCB callback);
MinimalDestroyCB MinimalSetDestroyCallback(MinimalApp* app, MinimalDestroyCB callback);
MinimalEventCB   MinimalSetEventCallback(MinimalApp* app, MinimalEventCB callback);
MinimalUpdateCB  MinimalSetUpdateCallback(MinimalApp* app, MinimalUpdateCB callback);
MinimalRenderCB  MinimalSetRenderCallback(MinimalApp* app, MinimalRenderCB callback);
MinimalRenderCB  MinimalSetRenderDebugCallback(MinimalApp* app, MinimalRenderCB callback);
MinimalRenderCB  MinimalSetRenderUICallback(MinimalApp* app, MinimalRenderCB callback);

/* --------------------------| settings |-------------------------------- */
void MinimalSetWindowTitle(MinimalApp* app, const char* title);

void MinimalEnableDebug(MinimalApp* app, int b);
void MinimalEnableVsync(MinimalApp* app, int b);

void MinimalToggleDebug(MinimalApp* app);
void MinimalToggleVsync(MinimalApp* app);

#endif // !MINIMAL_APPLICATION_H

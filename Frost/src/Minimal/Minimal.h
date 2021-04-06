#ifndef MINIMAL_APPLICATION_H
#define MINIMAL_APPLICATION_H

#include "MinimalTimer.h"
#include "MinimalEventHandler.h"

#include "MinimalInput.h"

#define APPLICATION_VER_STR_LEN 8
#define APPLICATION_STR_LEN		32
#define APPLICATION_PATH_LEN	64

#define EVENT_HANDLER_QUEUE_SIZE	64

typedef struct MinimalApp MinimalApp;
struct MinimalApp
{
	GLFWwindow* window;

	char* title;

	int width;
	int height;

	int running;
	int paused;

	int debug;
	int vsync;

	MinimalTimer timer;

	int (*on_init)(MinimalApp*);
	void (*on_destroy)(MinimalApp*);

	void (*on_event)(MinimalApp*, Event);
	void (*on_update)(MinimalApp*, float);
	void (*on_render)(MinimalApp*);
	void (*on_render_debug)(MinimalApp*);
	void (*on_render_gui)(MinimalApp*);
};

int MinimalLoad(MinimalApp* app, const char* title, int width, int height, char* gl_version);
int MinimalLoadConfig(MinimalApp* app, const char* path);
void MinimalDestroy(MinimalApp* app);

/* --------------------------| Game Loop |------------------------------- */
void MinimalRun(MinimalApp* app);
void MinimalPause(MinimalApp* app);
void MinimalClose(MinimalApp* app);

void MinimalSetOnInitCallback(MinimalApp* app, int (*callback)(MinimalApp*));
void MinimalSetOnDestroyCallback(MinimalApp* app, void (*callback)(MinimalApp*));
void MinimalSetOnEventCallback(MinimalApp* app, void (*callback)(MinimalApp*, Event));
void MinimalSetOnUpdateCallback(MinimalApp* app, void (*callback)(MinimalApp*, float));
void MinimalSetOnRenderCallback(MinimalApp* app, void (*callback)(MinimalApp*));
void MinimalSetOnRenderDebugCallback(MinimalApp* app, void (*callback)(MinimalApp*));

/* --------------------------| Settings |-------------------------------- */
void MinimalEnableDebug(MinimalApp* app, int b);
void MinimalEnableVsync(MinimalApp* app, int b);

void MinimalToggleDebug(MinimalApp* app);
void MinimalToggleVsync(MinimalApp* app);

void MinimalSetWindowTitle(MinimalApp* app, const char* title);
void MinimalSetWindowTitleFormat(MinimalApp* app, const char* fmt, ...);

#endif /* !MINIMAL_APPLICATION_H */
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Timer.h"
#include "EventHandler.h"

/* Graphics */
#include "Graphics/Renderer.h"
#include "Graphics/Resources.h"

#include "Input.h"
#include <GLFW/glfw3.h>

#include "defines.h"

typedef struct Application Application;
struct Application
{
	GLFWwindow* window;

	char* title;

	int width;
	int height;

	int running;
	int paused;

	int debug;
	int vsync;

	Timer timer;
	Resources resources;

	mat4 screen_projection;

	void (*on_init)(Application*);
	void (*on_destroy)(Application*);

	void (*on_event)(Application*, Event);
	void (*on_update)(Application*, float);
	void (*on_render)(Application*);
	void (*on_render_debug)(Application*);
	void (*on_render_gui)(Application*);
};

int ApplicationLoad(Application* app, const char* title, int width, int height, int glMajor, int glMinor, const char* res);
int ApplicationLoadConfig(Application* app, const char* path);
void ApplicationDestroy(Application* app);

/* --------------------------| Game Loop |------------------------------- */
void ApplicationRun(Application* app);
void ApplicationPause(Application* app);
void ApplicationClose(Application* app);

void ApplicationSetOnInitCallback(Application* app, void (*callback)(Application*));
void ApplicationSetOnDestroyCallback(Application* app, void (*callback)(Application*));
void ApplicationSetOnEventCallback(Application* app, void (*callback)(Application*, Event));
void ApplicationSetOnUpdateCallback(Application* app, void (*callback)(Application*, float));
void ApplicationSetOnRenderCallback(Application* app, void (*callback)(Application*));
void ApplicationSetOnRenderDebugCallback(Application* app, void (*callback)(Application*));

void ApplicationSetViewport(Application* app, int x, int y, int w, int h);
const float* ApplicationGetScreenProjPtr(Application* app);

/* --------------------------| Settings |-------------------------------- */
void ApplicationEnableDebugMode(Application* app, int b);
void ApplicationEnableVsync(Application* app, int b);

void ApplicationToggleDebugMode(Application* app);
void ApplicationToggleVsync(Application* app);

void ApplicationSetWindowTitle(Application* app, const char* title);
void ApplicationSetWindowTitleFormat(Application* app, const char* fmt, ...);

#endif /* !APPLICATION_H */
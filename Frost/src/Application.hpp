#pragma once

#include "Event/EventHandler.hpp"
#include "Obelisk/Obelisk.hpp"

#include "Input/Input.h"
#include "Timer.h"

// Renderer
#include "IgnisRenderer/FontRenderer.h"
#include "IgnisRenderer/Primitives2D.h"
#include "IgnisRenderer/BatchRenderer2D.h"
#include "IgnisRenderer/Renderer2D.h"

#include <GLFW/glfw3.h>

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

	int show_gui;

	Timer timer;

	void (*on_event)(Application*, const Event&);
	void (*on_update)(Application*, float);
	void (*on_render)(Application*);
	void (*on_render_debug)(Application*);
	void (*on_render_gui)(Application*);
};

int ApplicationLoad(Application* app, char* title, int width, int height, int glMajor, int glMinor);
void ApplicationDestroy(Application* app);

// --------------------------| Game Loop |-------------------------------
void ApplicationRun(Application* app);
void ApplicationPause(Application* app);
void ApplicationClose(Application* app);

void ApplicationSetOnEventCallback(Application* app, void (*callback)(Application*, const Event&));
void ApplicationSetOnUpdateCallback(Application* app, void (*callback)(Application*, float));
void ApplicationSetOnRenderCallback(Application* app, void (*callback)(Application*));
void ApplicationSetOnRenderDebugCallback(Application* app, void (*callback)(Application*));
void ApplicationSetOnRenderGuiCallback(Application* app, void (*callback)(Application*));

// --------------------------| Settings |--------------------------------
void ApplicationEnableDebugMode(Application* app, int b);
void ApplicationEnableVsync(Application* app, int b);
void ApplicationShowGui(Application* app, int b);

void ApplicationToggleDebugMode(Application* app);
void ApplicationToggleVsync(Application* app);
void ApplicationToggleGui(Application* app);

void ApplicationSetWindowTitle(Application* app, const char* title);
void ApplicationSetWindowTitleFormat(Application* app, const char* fmt, ...);

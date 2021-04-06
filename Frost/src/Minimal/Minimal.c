#include "Minimal.h"
#include "MinimalCallback.h"
#include "MinimalLogger.h"

#include "toolbox/toolbox.h"
#include "toolbox/tb_json.h"
#include "toolbox/tb_ini.h"
#include "toolbox/tb_file.h"
#include "toolbox/tb_str.h"

#include <stdlib.h>

static void MinimalGetGLVersion(char* version_str, int* major, int* minor)
{
	const char* sep = ".";
	char* maj_str = tb_strsep(&version_str, sep);
	char* min_str = tb_strsep(&version_str, sep);

	if (!maj_str || !min_str) return;

	*major = atoi(maj_str);
	*minor = atoi(min_str);
}

int MinimalLoad(MinimalApp* app, const char* title, int width, int height, char* gl_version)
{
	app->title = tb_strdup(title);
	if (!app->title)
	{
		MINIMAL_ERROR("[GLFW] Failed to allocate memory for title");
		return 0;
	}

	app->width = width;
	app->height = height;

	app->debug = 0;
	app->paused = 0;

	app->running = 0;

	/* GLFW initialization */
	if (glfwInit() == GLFW_FALSE)
	{
		MINIMAL_ERROR("[GLFW] Failed to initialize GLFW");
		glfwTerminate();
		return 0;
	}

	MINIMAL_INFO("[GLFW] Initialized GLFW %s", glfwGetVersionString());

	int gl_major, gl_minor;
	MinimalGetGLVersion(gl_version, &gl_major, &gl_minor);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	glfwSetErrorCallback(MinimalGLFWErrorCallback);

	MINIMAL_INFO("[GLFW] Created Window (%s) with size %d, %d", app->title, app->width, app->height);
	/* creating the window */
	app->window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (app->window == NULL)
	{
		MINIMAL_ERROR("[GLFW] Failed to create GLFW window");
		glfwTerminate();
		return 0;
	}


	glfwMakeContextCurrent(app->window);
	glfwSetWindowUserPointer(app->window, app);

	EventHandlerInit(EVENT_HANDLER_QUEUE_SIZE, MinimalEventCallback);

	/* Set GLFW callbacks */
	glfwSetWindowSizeCallback(app->window, MinimalGLFWWindowSizeCallback);
	glfwSetWindowCloseCallback(app->window, MinimalGLFWWindowCloseCallback);
	glfwSetKeyCallback(app->window, MinimalGLFWKeyCallback);
	glfwSetCharCallback(app->window, MinimalGLFWCharCallback);
	glfwSetMouseButtonCallback(app->window, MinimalGLFWMouseButtonCallback);
	glfwSetScrollCallback(app->window, MinimalGLFWScrollCallback);
	glfwSetCursorPosCallback(app->window, MinimalGLFWCursorPosCallback);

	MinimalTimerReset(&app->timer);

	app->running = (app->on_init) ? app->on_init(app) : 1;
	return app->running;
}

int MinimalLoadConfig(MinimalApp* app, const char* path)
{
	char* config = tb_file_read(path, "rb");

	if (!config)
	{
		MINIMAL_ERROR("Failed to read config (%s)", path);
		return 0;
	}

	tb_ini_element section;
	tb_ini_query(config, "app", &section);

	if (section.type == TB_INI_ERROR)
	{
		MINIMAL_ERROR("Failed to parse config (%s)", tb_ini_get_error_desc(section.error));
		return 0;
	}

	char title[APPLICATION_STR_LEN];
	tb_ini_query_string(section.start, ".title", title, APPLICATION_STR_LEN);

	int w = tb_ini_query_int(section.start, ".width", 0);
	int h = tb_ini_query_int(section.start, ".height", 0);

	char gl_version[APPLICATION_VER_STR_LEN];
	tb_ini_query_string(section.start, ".opengl", gl_version, APPLICATION_VER_STR_LEN);

	MinimalLoad(app, title, w, h, gl_version);
	
	/* apply settings */
	tb_ini_query(config, "options", &section);
	if (section.type != TB_INI_ERROR)
	{
		MinimalEnableDebug(app, tb_ini_query_bool(section.start, ".debug", 0));
		MinimalEnableVsync(app, tb_ini_query_bool(section.start, ".vsync", 0));
	}

	free(config);

	return app->running;
}

void MinimalDestroy(MinimalApp* app)
{
	if (app->on_destroy) app->on_destroy(app);

	EventHandlerDestroy();

	glfwDestroyWindow(app->window);
	glfwTerminate();
}

void MinimalRun(MinimalApp* app)
{
	/* game loop */
	while (app->running)
	{
		MinimalTimerStart(&app->timer, glfwGetTime());
		InputUpdate(app->window);

		if (!app->paused) app->on_update(app, (float)app->timer.deltatime);

		glClear(GL_COLOR_BUFFER_BIT);

		app->on_render(app);

		if (app->debug) app->on_render_debug(app);

		glfwPollEvents();
		EventHandlerPoll(app);
		glfwSwapBuffers(app->window);

		MinimalTimerEnd(&app->timer, glfwGetTime());
	}
}

void MinimalPause(MinimalApp* app)
{
	app->paused = !app->paused;

	if (app->paused)	MinimalSetWindowTitleFormat(app, "%s | Paused", app->title);
	else				MinimalSetWindowTitle(app, app->title);
}

void MinimalClose(MinimalApp* app) { app->running = 0; }

void MinimalSetOnInitCallback(MinimalApp* app, int(*callback)(MinimalApp*))
{
	app->on_init = callback;
}

void MinimalSetOnDestroyCallback(MinimalApp* app, void(*callback)(MinimalApp*))
{
	app->on_destroy = callback;
}

void MinimalSetOnEventCallback(MinimalApp* app, void(*callback)(MinimalApp*, Event))
{
	app->on_event = callback;
}

void MinimalSetOnUpdateCallback(MinimalApp* app, void(*callback)(MinimalApp*, float))
{
	app->on_update = callback;
}

void MinimalSetOnRenderCallback(MinimalApp* app, void(*callback)(MinimalApp*))
{
	app->on_render = callback;
}

void MinimalSetOnRenderDebugCallback(MinimalApp* app, void(*callback)(MinimalApp*))
{
	app->on_render_debug = callback;
}

void MinimalSetOnRenderGuiCallback(MinimalApp* app, void(*callback)(MinimalApp*))
{
	app->on_render_gui = callback;
}

void MinimalEnableDebug(MinimalApp* app, int b) { app->debug = b; }
void MinimalEnableVsync(MinimalApp* app, int b) { glfwSwapInterval(b); app->vsync = b; }

void MinimalToggleDebug(MinimalApp* app) { MinimalEnableDebug(app, !app->debug); }
void MinimalToggleVsync(MinimalApp* app) { MinimalEnableVsync(app, !app->vsync); }

void MinimalSetWindowTitle(MinimalApp* app, const char* title)
{ 
	glfwSetWindowTitle(app->window, title);
}

void MinimalSetWindowTitleFormat(MinimalApp* app, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	size_t buffer_size = vsnprintf(NULL, 0, fmt, args);

	char buffer[APPLICATION_STR_LEN];
	vsnprintf(buffer, tb_min64(buffer_size + 1, APPLICATION_STR_LEN), fmt, args);
	va_end(args);

	MinimalSetWindowTitle(app, buffer);
}
#include "Application.h"

#include <stdlib.h>

#include "ApplicationCallback.h"

#include "toolbox/tb_json.h"
#include "toolbox/tb_ini.h"
#include "toolbox/tb_file.h"
#include "toolbox/tb_str.h"

#include "Logger.h"

static void ApplicationGetGLVersion(char* version_str, int* major, int* minor)
{
	const char* sep = ".";
	char* maj_str = tb_strsep(&version_str, sep);
	char* min_str = tb_strsep(&version_str, sep);

	if (!maj_str || !min_str) return;

	*major = atoi(maj_str);
	*minor = atoi(min_str);
}

int ApplicationLoad(Application* app, const char* title, int width, int height, char* gl_version)
{
	app->title = tb_strdup(title);
	if (!app->title)
	{
		DEBUG_ERROR("[GLFW] Failed to allocate memory for title");
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
		DEBUG_ERROR("[GLFW] Failed to initialize GLFW");
		glfwTerminate();
		return 0;
	}

	DEBUG_INFO("[GLFW] Initialized GLFW %s", glfwGetVersionString());

	int gl_major, gl_minor;
	ApplicationGetGLVersion(gl_version, &gl_major, &gl_minor);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	/* creating the window */
	app->window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (app->window == NULL)
	{
		DEBUG_ERROR("[GLFW] Failed to create GLFW window");
		glfwTerminate();
		return 0;
	}

	glfwMakeContextCurrent(app->window);
	glfwSetWindowUserPointer(app->window, app);

	EventHandlerInit(EVENT_HANDLER_QUEUE_SIZE, ApplicationEventCallback);

	/* Set GLFW callbacks */
	glfwSetErrorCallback(ApplicationGLFWErrorCallback);
	glfwSetWindowSizeCallback(app->window, ApplicationGLFWWindowSizeCallback);
	glfwSetWindowCloseCallback(app->window, ApplicationGLFWWindowCloseCallback);
	glfwSetKeyCallback(app->window, ApplicationGLFWKeyCallback);
	glfwSetCharCallback(app->window, ApplicationGLFWCharCallback);
	glfwSetMouseButtonCallback(app->window, ApplicationGLFWMouseButtonCallback);
	glfwSetScrollCallback(app->window, ApplicationGLFWScrollCallback);
	glfwSetCursorPosCallback(app->window, ApplicationGLFWCursorPosCallback);

	TimerReset(&app->timer);

	app->running = (app->on_init) ? app->on_init(app) : 1;
	return app->running;
}

int ApplicationLoadConfig(Application* app, const char* path)
{
	char* config = tb_file_read(path, "rb", NULL);

	if (!config)
	{
		DEBUG_ERROR("Failed to read config (%s)", path);
		return 0;
	}

	tb_ini_element section;
	tb_ini_query(config, "app", &section);

	if (section.type == TB_INI_ERROR)
	{
		DEBUG_ERROR("Failed to parse config (%s)", tb_ini_get_error_desc(section.error));
		return 0;
	}

	char title[APPLICATION_STR_LEN];
	tb_ini_query_string(section.start, ".title", title, APPLICATION_STR_LEN);

	int w = tb_ini_query_int(section.start, ".width", 0);
	int h = tb_ini_query_int(section.start, ".height", 0);

	char gl_version[APPLICATION_VER_STR_LEN];
	tb_ini_query_string(section.start, ".opengl", gl_version, APPLICATION_VER_STR_LEN);

	ApplicationLoad(app, title, w, h, gl_version);
	
	/* apply settings */
	tb_ini_query(config, "options", &section);
	if (section.type != TB_INI_ERROR)
	{
		ApplicationEnableDebug(app, tb_ini_query_bool(section.start, ".debug", 0));
		ApplicationEnableVsync(app, tb_ini_query_bool(section.start, ".vsync", 0));
	}

	free(config);

	return app->running;
}

void ApplicationDestroy(Application* app)
{
	if (app->on_destroy) app->on_destroy(app);

	EventHandlerDestroy();

	glfwDestroyWindow(app->window);
	glfwTerminate();
}

void ApplicationRun(Application* app)
{
	/* game loop */
	while (app->running)
	{
		TimerStart(&app->timer, glfwGetTime());
		InputUpdate(app->window);

		if (!app->paused) app->on_update(app, (float)app->timer.deltatime);

		glClear(GL_COLOR_BUFFER_BIT);

		app->on_render(app);

		if (app->debug) app->on_render_debug(app);

		glfwPollEvents();
		EventHandlerPoll(app);
		glfwSwapBuffers(app->window);

		TimerEnd(&app->timer, glfwGetTime());
	}
}

void ApplicationPause(Application* app)
{
	app->paused = !app->paused;

	if (app->paused)
		ApplicationSetWindowTitleFormat(app, "%s | Paused", app->title);
	else
		ApplicationSetWindowTitle(app, app->title);
}

void ApplicationClose(Application* app) { app->running = 0; }

void ApplicationSetOnInitCallback(Application* app, int(*callback)(Application*))
{
	app->on_init = callback;
}

void ApplicationSetOnDestroyCallback(Application* app, void(*callback)(Application*))
{
	app->on_destroy = callback;
}

void ApplicationSetOnEventCallback(Application* app, void(*callback)(Application*, Event))
{
	app->on_event = callback;
}

void ApplicationSetOnUpdateCallback(Application* app, void(*callback)(Application*, float))
{
	app->on_update = callback;
}

void ApplicationSetOnRenderCallback(Application* app, void(*callback)(Application*))
{
	app->on_render = callback;
}

void ApplicationSetOnRenderDebugCallback(Application* app, void(*callback)(Application*))
{
	app->on_render_debug = callback;
}

void ApplicationSetOnRenderGuiCallback(Application* app, void(*callback)(Application*))
{
	app->on_render_gui = callback;
}

void ApplicationEnableDebug(Application* app, int b) { app->debug = b; }
void ApplicationEnableVsync(Application* app, int b) { glfwSwapInterval(b); app->vsync = b; }

void ApplicationToggleDebug(Application* app) { ApplicationEnableDebug(app, !app->debug); }
void ApplicationToggleVsync(Application* app) { ApplicationEnableVsync(app, !app->vsync); }

void ApplicationSetWindowTitle(Application* app, const char* title) 
{ 
	glfwSetWindowTitle(app->window, title);
}

void ApplicationSetWindowTitleFormat(Application* app, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	size_t buffer_size = vsnprintf(NULL, 0, fmt, args);
	char* buffer = malloc(buffer_size + 1);
	vsnprintf(buffer, buffer_size + 1, fmt, args);
	va_end(args);

	ApplicationSetWindowTitle(app, buffer);

	free(buffer);
}
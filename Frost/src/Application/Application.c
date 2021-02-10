#include "Application.h"

#include "ApplicationCallback.h"

#include "toolbox/tb_json.h"
#include "toolbox/tb_file.h"

#include "Debugger.h"
#include "defines.h"

int ApplicationLoad(Application* app, const char* title, int width, int height, int glMajor, int glMinor)
{
	app->title = malloc(strlen(title));
	if (!app->title)
	{
		DEBUG_ERROR("[GLFW] Failed to allocate memory for title\n");
		return 0;
	}
	strcpy(app->title, title);

	app->width = width;
	app->height = height;

	app->debug = 0;
	app->paused = 0;

	app->running = 0;

	/* GLFW initialization */
	if (glfwInit() == GLFW_FALSE)
	{
		DEBUG_ERROR("[GLFW] Failed to initialize GLFW\n");
		glfwTerminate();
		return 0;
	}

	DEBUG_INFO("[GLFW] Initialized GLFW %s\n", glfwGetVersionString());

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	/* creating the window */
	app->window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (app->window == NULL)
	{
		DEBUG_ERROR("[GLFW] Failed to create GLFW window\n");
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

	ignisSetErrorCallback(ApplicationIgnisErrorCallback);

	int debug = 0;

#ifdef _DEBUG
	debug = 1;
#endif /* !_DEBUG */

	/* ingis initialization */
	if (!ignisInit(debug))
	{
		DEBUG_ERROR("[IGNIS] Failed to initialize Ignis\n");
		glfwTerminate();
		return 0;
	}

	DEBUG_INFO("[OpenGL] Version: %s\n", ingisGetGLVersion());
	DEBUG_INFO("[OpenGL] Vendor: %s\n", ingisGetGLVendor());
	DEBUG_INFO("[OpenGL] Renderer: %s\n", ingisGetGLRenderer());
	DEBUG_INFO("[OpenGL] GLSL Version: %s\n", ingisGetGLSLVersion());

	ApplicationSetViewport(app, 0, 0, app->width, app->height);
	TimerReset(&app->timer);

	app->running = (app->on_init) ? app->on_init(app) : 1;
	return app->running;
}

int ApplicationLoadConfig(Application* app, const char* path)
{
	char* json = tb_file_read(path, "rb", NULL);

	if (!json)
	{
		DEBUG_ERROR("Failed to read config (%s)\n", path);
		return 0;
	}

	tb_json_element element;
	tb_json_read(json, &element, "{'app'");

	char title[APPLICATION_STR_LEN];
	tb_json_string((char*)element.value, "{'title'", title, APPLICATION_STR_LEN, NULL);

	int width = tb_json_int((char*)element.value, "{'width'", NULL, 0);
	int height = tb_json_int((char*)element.value, "{'height'", NULL, 0);

	int major = tb_json_int((char*)element.value, "{'opengl'[0", NULL, 0);
	int minor = tb_json_int((char*)element.value, "{'opengl'[1", NULL, 0);

	free(json);

	return ApplicationLoad(app, title, width, height, major, minor);
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

		if (!app->paused)
			app->on_update(app, (float)app->timer.deltatime);

		glClear(GL_COLOR_BUFFER_BIT);

		app->on_render(app);

		if (app->debug)
			app->on_render_debug(app);

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

void ApplicationSetOnEventCallback(Application* app, void(*callback)(Application*, const Event))
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

void ApplicationSetViewport(Application* app, int x, int y, int w, int h)
{
	app->screen_projection = mat4_ortho((float)x, (float)w, (float)h, (float)y, -1.0f, 1.0f);
	glViewport(x, y, w, h);
}

const float* ApplicationGetScreenProjPtr(const Application* app)
{
	return app->screen_projection.v;
}

vec2 ApplicationGetScreenSize(const Application* app)
{
	return (vec2) { (float)app->width, (float)app->height };
}

void ApplicationEnableDebugMode(Application* app, int b) { app->debug = b; }
void ApplicationEnableVsync(Application* app, int b) { glfwSwapInterval(b); app->vsync = b; }

void ApplicationToggleDebugMode(Application* app) { ApplicationEnableDebugMode(app, !app->debug); }
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
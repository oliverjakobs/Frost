#include "Application.hpp"

static void _ApplicationSetGLFWCallback(Application* app)
{
	glfwSetErrorCallback([](int error, const char* desc)
	{
		OBELISK_ERROR("[GLFW] ({0}) {1}", error, desc);
	});

	glfwSetWindowSizeCallback(app->window, [](GLFWwindow* window, int width, int height)
	{
		auto game = (Application*)glfwGetWindowUserPointer(window);

		game->width = width;
		game->height = height;

		EventHandler::Throw<WindowResizeEvent>(width, height);
	});

	glfwSetWindowCloseCallback(app->window, [](GLFWwindow* window)
	{
		EventHandler::Throw<WindowCloseEvent>();
	});

	glfwSetKeyCallback(app->window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		switch (action)
		{
		case GLFW_PRESS:
			EventHandler::Throw<KeyPressedEvent>(key, 0);
			break;
		case GLFW_RELEASE:
			EventHandler::Throw<KeyReleasedEvent>(key);
			break;
		case GLFW_REPEAT:
			EventHandler::Throw<KeyPressedEvent>(key, 1);
			break;
		}
	});

	glfwSetCharCallback(app->window, [](GLFWwindow* window, unsigned int keycode)
	{
		EventHandler::Throw<KeyTypedEvent>(keycode);
	});

	glfwSetMouseButtonCallback(app->window, [](GLFWwindow* window, int button, int action, int mods)
	{
		auto game = (Application*)glfwGetWindowUserPointer(window);

		switch (action)
		{
		case GLFW_PRESS:
			EventHandler::Throw<MouseButtonPressedEvent>(button);
			break;
		case GLFW_RELEASE:
			EventHandler::Throw<MouseButtonReleasedEvent>(button);
			break;
		}
	});

	glfwSetScrollCallback(app->window, [](GLFWwindow* window, double xOffset, double yOffset)
	{
		EventHandler::Throw<MouseScrolledEvent>((float)xOffset, (float)yOffset);
	});

	glfwSetCursorPosCallback(app->window, [](GLFWwindow* window, double xPos, double yPos)
	{
		EventHandler::Throw<MouseMovedEvent>((float)xPos, (float)yPos);
	});
}

static int _ApplicationOnWindowClose(Application* app, WindowCloseEvent& e)
{
	app->running = 0;
	return 1;
}

// gets called when an event is processed, handles some (e.g. WindowCloseEvents) 
// and passes the rest to OnEvent, which is implemented in the game class
void _ApplicationEventCallback(Application* app, Event& e)
{
	switch (e.Type)
	{
	case EventType::WindowClose:
		_ApplicationOnWindowClose(app, (WindowCloseEvent&)e);
		break;
	case EventType::ChangeScene:
		//m_sceneManager.ChangeScene(((ChangeSceneEvent&)e).GetTarget());
		break;
	default:
		app->on_event(app, e);
		break;
	}
}

int ApplicationLoad(Application* app, char* title, int width, int height, int glMajor, int glMinor)
{
	app->title = title;
	app->width = width;
	app->height = height;

	app->debug = 0;
	app->paused = 0;

	app->show_gui = 0;

	app->running = 0;

	// GLFW initialization
	if (glfwInit() == GLFW_FALSE)
	{
		OBELISK_ERROR("[GLFW] Failed to initialize GLFW");
		glfwTerminate();
		return 0;
	}

	OBELISK_INFO("[GLFW] Initialized GLFW %s", glfwGetVersionString());

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	// creating the window
	app->window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (app->window == NULL)
	{
		OBELISK_ERROR("[GLFW] Failed to create GLFW window");
		glfwTerminate();
		return 0;
	}

	glfwMakeContextCurrent(app->window);
	glfwSetWindowUserPointer(app->window, app);

	EventHandler::SetEventCallback(_ApplicationEventCallback);

	// Set GLFW callbacks
	_ApplicationSetGLFWCallback(app);

	ignisSetErrorCallback([](ignisErrorLevel level, const char* desc)
	{
		switch (level)
		{
		case IGNIS_WARN:		OBELISK_WARN(desc); break;
		case IGNIS_ERROR:		OBELISK_ERROR(desc); break;
		case IGNIS_CRITICAL:	OBELISK_CRITICAL(desc); break;
		}
	});

	int debug = 0;

#ifdef _DEBUG
	debug = 1;
#endif // _DEBUG

	// ingis initialization
	if (!ignisInit(debug))
	{
		OBELISK_ERROR("[IGNIS] Failed to initialize Ignis");
		glfwTerminate();
		return 0;
	}

	OBELISK_INFO("[OpenGL] Version: %s", glGetString(GL_VERSION));
	OBELISK_INFO("[OpenGL] Vendor: %s", glGetString(GL_VENDOR));
	OBELISK_INFO("[OpenGL] Renderer: %s", glGetString(GL_RENDERER));
	OBELISK_INFO("[OpenGL] GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

	TimerReset(&app->timer);

	app->running = 1;
	return 1;
}

void ApplicationDestroy(Application* app)
{
	glfwDestroyWindow(app->window);
	glfwTerminate();
}

void ApplicationRun(Application* app)
{
	// Game loop
	while (app->running)
	{
		TimerStart(&app->timer, glfwGetTime());

		if (!app->paused)
			app->on_update(app, (float)app->timer.deltatime);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		app->on_render(app);

		if (app->debug)
			app->on_render_debug(app);

		if (app->show_gui)
			app->on_render_gui(app);

		glfwPollEvents();
		EventHandler::Poll(app);
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

void ApplicationClose(Application* app) { app->running = false; }

void ApplicationSetOnEventCallback(Application* app, void(*callback)(Application*, const Event&))
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

void ApplicationEnableDebugMode(Application* app, int b) { app->debug = b; }
void ApplicationEnableVsync(Application* app, int b) { glfwSwapInterval(b); app->vsync = b; }
void ApplicationShowGui(Application* app, int b) { app->show_gui = b; }

void ApplicationToggleDebugMode(Application* app) { ApplicationEnableDebugMode(app, !app->debug); }
void ApplicationToggleVsync(Application* app) { ApplicationEnableVsync(app, !app->vsync); }
void ApplicationToggleGui(Application* app) { ApplicationShowGui(app, !app->show_gui); }

void ApplicationSetWindowTitle(Application* app, const char* title) { glfwSetWindowTitle(app->window, title); }

void ApplicationSetWindowTitleFormat(Application* app, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	size_t buffer_size = vsnprintf(NULL, 0, fmt, args);
	char* buffer = (char*)malloc(buffer_size + 1);
	vsnprintf(buffer, buffer_size + 1, fmt, args);
	va_end(args);

	ApplicationSetWindowTitle(app, buffer);

	free(buffer);
}
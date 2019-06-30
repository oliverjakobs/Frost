#include "Application.h"

#include <GLFW/glfw3.h>

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	DEBUG_INFO("OpenGL debug output ({0}): {1}", id, message);

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             DEBUG_INFO("Source: API"); break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   DEBUG_INFO("Source: Window System"); break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: DEBUG_INFO("Source: Shader Compiler"); break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     DEBUG_INFO("Source: Third Party"); break;
	case GL_DEBUG_SOURCE_APPLICATION:     DEBUG_INFO("Source: Application"); break;
	case GL_DEBUG_SOURCE_OTHER:           DEBUG_INFO("Source: Other"); break;
	}

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               DEBUG_INFO("Type: Error"); break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: DEBUG_INFO("Type: Deprecated Behaviour"); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  DEBUG_INFO("Type: Undefined Behaviour"); break;
	case GL_DEBUG_TYPE_PORTABILITY:         DEBUG_INFO("Type: Portability"); break;
	case GL_DEBUG_TYPE_PERFORMANCE:         DEBUG_INFO("Type: Performance"); break;
	case GL_DEBUG_TYPE_MARKER:              DEBUG_INFO("Type: Marker"); break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          DEBUG_INFO("Type: Push Group"); break;
	case GL_DEBUG_TYPE_POP_GROUP:           DEBUG_INFO("Type: Pop Group"); break;
	case GL_DEBUG_TYPE_OTHER:               DEBUG_INFO("Type: Other"); break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         DEBUG_INFO("Severity: high"); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       DEBUG_INFO("Severity: medium"); break;
	case GL_DEBUG_SEVERITY_LOW:          DEBUG_INFO("Severity: low"); break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: DEBUG_INFO("Severity: notification"); break;
	}
}


bool Application::OnWindowClose(WindowCloseEvent& e)
{
	Close();
	return true;
}

void Application::EventCallback(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

	OnEvent(e);
}

Application::Application(const std::string& title, int width, int height)
	: m_title(title), m_width(width), m_height(height)
{
	// GLFW initialization
	if (glfwInit() == GLFW_FALSE)
	{
		DEBUG_ERROR("Failed to initialize GLFW");
		glfwTerminate();
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	DEBUG_INFO("Initialized GLFW {0}", glfwGetVersionString());

	m_debug = false;

	// creating the window
	m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (m_window == nullptr)
	{
		DEBUG_ERROR("Failed to create GLFW window");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, this);

	DEBUG_INFO("Window created: {0} ({1}, {2})", title, width, height);

	// Set GLFW callbacks
	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
	{
		auto game = (Application*)glfwGetWindowUserPointer(window);

		game->m_width = width;
		game->m_height = height;

		glViewport(0, 0, width, height);

		WindowResizeEvent event(width, height);
		game->EventCallback(event);
	});

	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
	{
		auto game = (Application*)glfwGetWindowUserPointer(window);

		WindowCloseEvent event;
		game->EventCallback(event);
	});

	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		auto game = (Application*)glfwGetWindowUserPointer(window);

		switch (action)
		{
		case GLFW_PRESS:
		{
			KeyPressedEvent event(key, 0);
			game->EventCallback(event);
			break;
		}
		case GLFW_RELEASE:
		{
			KeyReleasedEvent event(key);
			game->EventCallback(event);
			break;
		}
		case GLFW_REPEAT:
		{
			KeyPressedEvent event(key, 1);
			game->EventCallback(event);
			break;
		}
		}
	});

	glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode)
	{
		auto game = (Application*)glfwGetWindowUserPointer(window);

		KeyTypedEvent event(keycode);
		game->EventCallback(event);
	});

	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
	{
		auto game = (Application*)glfwGetWindowUserPointer(window);

		switch (action)
		{
		case GLFW_PRESS:
		{
			MouseButtonPressedEvent event(button);
			game->EventCallback(event);
			break;
		}
		case GLFW_RELEASE:
		{
			MouseButtonReleasedEvent event(button);
			game->EventCallback(event);
			break;
		}
		}
	});

	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
	{
		auto game = (Application*)glfwGetWindowUserPointer(window);

		MouseScrolledEvent event((float)xOffset, (float)yOffset);
		game->EventCallback(event);
	});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
	{
		auto game = (Application*)glfwGetWindowUserPointer(window);

		MouseMovedEvent event((float)xPos, (float)yPos);
		game->EventCallback(event);
	});

	// loading glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		DEBUG_ERROR("Failed to initialize GLAD");
		glfwTerminate();
		return;
	}

	DEBUG_INFO("Initialized GLAD");
	DEBUG_INFO("OpenGL Version: {0}", glGetString(GL_VERSION));
	DEBUG_INFO("OpenGL Vendor: {0}", glGetString(GL_VENDOR));
	DEBUG_INFO("OpenGL Renderer: {0}", glGetString(GL_RENDERER));
	DEBUG_INFO("GLSL Version: {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));

#ifdef _DEBUG
	//Set up opengl debug output
	GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

		DEBUG_INFO("Debug context created");
	}
#endif

	Renderer::Init(0.0f, 0.0f, (float)m_width, (float)m_height);

	// initialize imgui
	//m_imguiRenderer.Init(m_window, ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable);
	m_imguiRenderer.Init(m_window, ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable);

	m_running = true;
}

Application::~Application()
{
	Renderer::Destroy();

	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Application::EnableDebugMode(bool b)
{
	m_debug = b;
}

void Application::ToggleDebugMode()
{
	m_debug = !m_debug;
}

void Application::EnableVsync(bool b)
{
	glfwSwapInterval(b);
}

void Application::Run()
{
	// Game loop
	while (m_running)
	{
		Timer::Start();

		OnUpdate();

		Renderer::Start();
		m_imguiRenderer.Start();

		OnRender();

		if (m_debug)
			OnRenderDebug();

		Renderer::Flush();
		m_imguiRenderer.Flush(m_window);

		glfwPollEvents();
		glfwSwapBuffers(m_window);

		Timer::End();
	}
}

void Application::Close()
{
	m_running = false;
}

void Application::SetTitle(const std::string& title)
{
	glfwSetWindowTitle(m_window, title.c_str());
}

int Application::GetWidth() const
{
	return m_width;
}

int Application::GetHeight() const
{
	return m_height;
}

float Application::GetWidthF() const
{
	return static_cast<float>(m_width);
}

float Application::GetHeightF() const
{
	return static_cast<float>(m_height);
}

GLFWwindow* Application::GetContext() const
{
	return m_window;
}

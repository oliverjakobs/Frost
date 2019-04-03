#include "Application.h"

#include "Log/Logger.h"

Application::Application(const std::string& title, int width, int height)
	: m_title(title), m_width(width), m_height(height)
{
	// GLFW initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	DEBUG_INFO("Initialized glfw");

	m_debug = false;

	// creating the window
	m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (m_window == NULL)
	{
		DEBUG_ERROR("Failed to create GLFW window");
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_window);

	glfwSetWindowUserPointer(m_window, this);

	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
	{
		auto game = (Application*)glfwGetWindowUserPointer(window);

		game->m_width = width;
		game->m_height = height;

		glViewport(0, 0, width, height);
	});

	DEBUG_INFO("Window created");

	// loading glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		DEBUG_ERROR("Failed to initialize GLAD");
		glfwTerminate();
		return;
	}

	DEBUG_INFO("Initialized GLAD");

	Renderer::Init(0.0f, 0.0f, (float)m_width, (float)m_height);
}

Application::~Application()
{
	Renderer::Destroy();

	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Application::close()
{
	glfwSetWindowShouldClose(m_window, true);
}

void Application::setDebugMode(bool b)
{
	m_debug = b;
}

void Application::toggleDebugMode()
{
	m_debug = !m_debug;
}

void Application::enableVsync(bool b)
{
	glfwSwapInterval(b);
}

void Application::run()
{
	while (!glfwWindowShouldClose(m_window))
	{
		Timer::Start();

		glfwPollEvents();

		onInput();
		onUpdate();
		Input::OnUpdate(m_window);

		Renderer::Start();

		onRender();

		if (m_debug)
			onRenderDebug();

		Renderer::Flush();

		glfwSwapBuffers(m_window);

		Timer::End();
	}
}

void Application::setTitle(const std::string& title)
{
	glfwSetWindowTitle(m_window, title.c_str());
}

int Application::getWidth() const
{
	return m_width;
}

int Application::getHeight() const
{
	return m_height;
}

float Application::getWidthF() const
{
	return static_cast<float>(m_width);
}

float Application::getHeightF() const
{
	return static_cast<float>(m_height);
}

GLFWwindow* Application::getContext() const
{
	return m_window;
}

#include "Scrapbook.h"

namespace sb
{
	Scrapbook::Scrapbook(const std::string& title, int width, int height)
	{
		m_data.title = title;
		m_data.width = width;
		m_data.height = height;

		// GLFW initialization
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_debug = false;

		// creating the window
		m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		if (m_window == NULL)
		{
			DEBUG_MESSAGE("Failed to create GLFW window");
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(m_window);

		glfwSetWindowUserPointer(m_window, &m_data);

		glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
		{
			auto data = (WindowData*)glfwGetWindowUserPointer(window);

			data->width = width;
			data->height = height;

			glViewport(0, 0, width, height);
		});

		// loading glad
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			DEBUG_MESSAGE("Failed to initialize GLAD");
			glfwTerminate();
			return;
		}

		Renderer::Init(0.0f, 0.0f, (float)width, (float)height);
	}

	Scrapbook::~Scrapbook()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void Scrapbook::close()
	{
		glfwSetWindowShouldClose(m_window, true);
	}

	void Scrapbook::setDebugMode(bool b)
	{
		m_debug = b;
	}

	void Scrapbook::toggleDebugMode()
	{
		m_debug = !m_debug;
	}

	void Scrapbook::run()
	{
		while (!glfwWindowShouldClose(m_window))
		{
			Timer::Start(glfwGetTime());

			onInput();
			onUpdate();

			Renderer::Start();

			onRender();

			if (m_debug)
				onRenderDebug();

			Renderer::Flush();

			glfwSwapBuffers(m_window);

			Timer::End(glfwGetTime());

			glfwPollEvents();
		}
	}

	void Scrapbook::setClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void Scrapbook::setClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Scrapbook::setTitle(const std::string& title)
	{
		glfwSetWindowTitle(m_window, title.c_str());
	}
	
	int Scrapbook::getWidth() const
	{
		return m_data.width;
	}

	int Scrapbook::getHeight() const
	{
		return m_data.height;
	}

	GLFWwindow* Scrapbook::getContext() const
	{
		return m_window;
	}
}

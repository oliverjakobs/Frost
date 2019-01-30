#include "Scrapbook.h"

#include <iostream>

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

		// creating the window
		m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		if (m_window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
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
			std::cout << "Failed to initialize GLAD" << std::endl;
			glfwTerminate();
			return;
		}
	}

	Scrapbook::~Scrapbook()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void Scrapbook::run()
	{
		while (!glfwWindowShouldClose(m_window))
		{
			Timer::Start(glfwGetTime());

			onInput();
			onUpdate();

			glClear(GL_COLOR_BUFFER_BIT);

			onRender();

			glBindVertexArray(0);

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

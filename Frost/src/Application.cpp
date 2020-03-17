#include "Application.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

static glm::mat4 s_screenMat = glm::mat4(1.0f);

void SetViewport(int x, int y, int w, int h)
{
	s_screenMat = glm::ortho((float)x, (float)w, (float)h, (float)y);

	glViewport(x, y, w, h);
}

const glm::mat4& GetScreenMat()
{
	return s_screenMat;
}

void Application::SetGLFWCallback()
{
	glfwSetErrorCallback([](int error, const char* desc)
	{
		OBELISK_ERROR("[GLFW] ({0}) {1}", error, desc);
	});

	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
	{
		auto game = (Application*)glfwGetWindowUserPointer(window);

		game->m_width = width;
		game->m_height = height;

		SetViewport(0, 0, width, height);

		EventHandler::Throw<WindowResizeEvent>(width, height);
	});

	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
	{
		EventHandler::Throw<WindowCloseEvent>();
	});

	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
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

	glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode)
	{
		EventHandler::Throw<KeyTypedEvent>(keycode);
	});

	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
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

	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
	{
		EventHandler::Throw<MouseScrolledEvent>((float)xOffset, (float)yOffset);
	});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
	{
		EventHandler::Throw<MouseMovedEvent>((float)xPos, (float)yPos);
	});
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
	Close();
	return true;
}

void Application::EventCallback(Event& e)
{
	switch (e.GetType())
	{
	case EventType::WindowClose:
		OnWindowClose((WindowCloseEvent&)e);
		break;
	case EventType::ChangeScene:
		//m_sceneManager.ChangeScene(((ChangeSceneEvent&)e).GetTarget());
		break;
	default:
		OnEvent(e);
		break;
	}
}

bool Application::LoadApplication(const std::string& title, int width, int height, int glMajor, int glMinor)
{
	m_title = title;
	m_width = width;
	m_height = height;

	m_debug = false;
	m_paused = false;
	m_showImGui = false;

	// GLFW initialization
	if (glfwInit() == GLFW_FALSE)
	{
		OBELISK_ERROR("[GLFW] Failed to initialize GLFW");
		glfwTerminate();
		return false;
	}

	OBELISK_INFO("[GLFW] Initialized GLFW %s", glfwGetVersionString());

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	// creating the window
	m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (m_window == nullptr)
	{
		OBELISK_ERROR("[GLFW] Failed to create GLFW window");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, this);

	EventHandler::SetEventCallback(BIND_FUNCTION(Application::EventCallback));

	// Set GLFW callbacks
	SetGLFWCallback();

	s_screenMat = glm::ortho(0.0f, (float)width, (float)height, 0.0f);

	ignisSetErrorCallback([](ignisErrorLevel level, const char* desc)
	{
		switch (level)
		{
		case IGNIS_WARN:		OBELISK_WARN(desc); break;
		case IGNIS_ERROR:		OBELISK_ERROR(desc); break;
		case IGNIS_CRITICAL:	OBELISK_CRITICAL(desc); break;
		}
	});

	bool debug = false;
	
#ifdef _DEBUG
	debug = true;
#endif // _DEBUG

	// ingis initialization
	if (!ignisInit(debug))
	{
		OBELISK_ERROR("[IGNIS] Failed to initialize Ignis");
		glfwTerminate();
		return nullptr;
	}

	OBELISK_INFO("[OpenGL] Version: %s", glGetString(GL_VERSION));
	OBELISK_INFO("[OpenGL] Vendor: %s", glGetString(GL_VENDOR));
	OBELISK_INFO("[OpenGL] Renderer: %s", glGetString(GL_RENDERER));
	OBELISK_INFO("[OpenGL] GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// initialize imgui
	//ImGuiRenderer::Init(m_window, ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable);
	ImGuiRenderer::Init(m_window, ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable);

	return true;
}

Application::Application(const std::string& title, int width, int height)
{
	m_running = LoadApplication(title, width, height, 4, 4);
}

Application::~Application()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Application::EnableDebugMode(bool b) { m_debug = b; }
void Application::EnableImGui(bool b) { m_showImGui = b; }
void Application::EnableVsync(bool b) { glfwSwapInterval(b); m_vsync = b; }

void Application::ToggleDebugMode() { EnableDebugMode(!m_debug); }
void Application::ToggleImGui() { EnableImGui(!m_showImGui); }
void Application::ToggleVsync() { EnableVsync(!m_vsync); }

void Application::Pause() 
{
	m_paused = !m_paused;

	if (m_paused)
		SetTitle(m_title + " | Paused");
	else
		SetTitle(m_title);
}


void Application::Run()
{
	// Game loop
	while (m_running)
	{
		m_timer.Start((float)glfwGetTime());

		if (!m_paused)
			OnUpdate(m_timer.DeltaTime);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		OnRender();

		if (m_debug)
			OnRenderDebug();

		if (m_showImGui)
		{
			ImGuiRenderer::Begin();
			OnImGui();
			ImGuiRenderer::End();
		}

		glfwPollEvents();
		EventHandler::Poll();
		glfwSwapBuffers(m_window);

		m_timer.End((float)glfwGetTime());
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
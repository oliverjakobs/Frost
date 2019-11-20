#include "Application.h"

#include <GLFW/glfw3.h>

bool Application::OnWindowClose(WindowCloseEvent& e)
{
	Close();
	return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{
	m_camera.SetProjection(0.0f, (float)e.GetWidth(), 0.0f, (float)e.GetHeight());
	return true;
}

void Application::EventCallback(Event& e)
{
	switch (e.GetType())
	{
	case EventType::WindowClose:
		OnWindowClose((WindowCloseEvent&)e);
		break;
	case EventType::WindowResize:
		OnWindowResize((WindowResizeEvent&)e);
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

	// ingis initialization
	if (!ignis::ignisInit(width, height))
	{
		OBELISK_ERROR("[Ignis] Failed to initialize Ignis");
		return nullptr;
	}

	ignisSetErrorCallback([](ignisErrorLevel level, const std::string& desc)
	{
		switch (level)
		{
		case ignisErrorLevel::Warn:		OBELISK_WARN(desc.c_str()); break;
		case ignisErrorLevel::Error:	OBELISK_ERROR(desc.c_str()); break;
		case ignisErrorLevel::Critical:	OBELISK_CRITICAL(desc.c_str()); break;
		}
	});

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
	glfwSetErrorCallback([](int error, const char* desc)
	{
		OBELISK_ERROR("[GLFW] ({0}) {1}", error, desc);
	});

	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
	{
		auto game = (Application*)glfwGetWindowUserPointer(window);

		game->m_width = width;
		game->m_height = height;

		ignis::ignisViewport(0, 0, width, height);

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

	bool debug = true;

	if (!ignis::ignisLoadGL(debug))
	{
		OBELISK_ERROR("[IGNIS] Failed to load OpenGL");
		glfwTerminate();
		return nullptr;
	}

	OBELISK_INFO("[OpenGL] Version: %s", glGetString(GL_VERSION));
	OBELISK_INFO("[OpenGL] Vendor: %s", glGetString(GL_VENDOR));
	OBELISK_INFO("[OpenGL] Renderer: %s", glGetString(GL_RENDERER));
	OBELISK_INFO("[OpenGL] GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

	m_camera = ignis::OrthographicCamera(0.0f, (float)width, 0.0f, (float)height);

	// initialize imgui
	//ImGuiRenderer::Init(m_window, ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable);
	ImGuiRenderer::Init(m_window, ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable);

	return true;
}

Application::Application(const std::string& config)
{
	//OBELISK_ASSERT(!config.empty(), "Path is emtpy");
	//
	//json root = jsonParseFile(config);
	//
	//// ---------------| window |------------------------------------------
	//std::string title;
	//
	//int width;
	//int height;
	//
	//int glMajor = 4;
	//int glMinor = 0;
	//
	//if (root.find("window") != root.end())
	//{
	//	json window = root.at("window");
	//
	//	title = jsonToString(window, "title");
	//	width = jsonToInt(window, "width");
	//	height = jsonToInt(window, "height");
	//}
	//
	//// ---------------| gl version |--------------------------------------
	//if (root.find("opengl") != root.end())
	//{
	//	json opengl = root.at("opengl");
	//
	//	glMajor = jsonToInt(opengl, "major");
	//	glMinor = jsonToInt(opengl, "minor");
	//}
	//
	//m_running = LoadApplication(title, width, height, glMajor, glMinor);
	//
	//// ---------------| load resources |----------------------------------
	//std::string resources = jsonToString(root, "resources");
	//
	//if (!resources.empty())
	//	ResourceManager::Load(resources);
}

Application::Application(const std::string& title, int width, int height)
{
	m_running = LoadApplication(title, width, height, 4, 5);
}

Application::~Application()
{
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

void Application::EnableImGui(bool b)
{
	m_showImGui = b;
}

void Application::ToggleImGui()
{
	m_showImGui = !m_showImGui;
}

void Application::Pause()
{
	m_paused = !m_paused;

	if (m_paused)
		SetTitle(m_title + " | Paused");
	else
		SetTitle(m_title);
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
		m_timer.Start((float)glfwGetTime());

		if (!m_paused)
			OnUpdate(m_timer.DeltaTime);

		glClear(GL_COLOR_BUFFER_BIT);

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
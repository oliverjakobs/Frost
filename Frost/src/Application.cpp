#include "Application.h"

#include <GLFW/glfw3.h>

#include "Script/JSONParser.h"

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	DEBUG_INFO("OpenGL debug output ({0}): {1}", id, message);

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:				DEBUG_INFO("Source: API"); break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		DEBUG_INFO("Source: Window System"); break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:	DEBUG_INFO("Source: Shader Compiler"); break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:		DEBUG_INFO("Source: Third Party"); break;
	case GL_DEBUG_SOURCE_APPLICATION:		DEBUG_INFO("Source: Application"); break;
	case GL_DEBUG_SOURCE_OTHER:				DEBUG_INFO("Source: Other"); break;
	}

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:				DEBUG_INFO("Type: Error"); break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	DEBUG_INFO("Type: Deprecated Behaviour"); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	DEBUG_INFO("Type: Undefined Behaviour"); break;
	case GL_DEBUG_TYPE_PORTABILITY:			DEBUG_INFO("Type: Portability"); break;
	case GL_DEBUG_TYPE_PERFORMANCE:			DEBUG_INFO("Type: Performance"); break;
	case GL_DEBUG_TYPE_MARKER:				DEBUG_INFO("Type: Marker"); break;
	case GL_DEBUG_TYPE_PUSH_GROUP:			DEBUG_INFO("Type: Push Group"); break;
	case GL_DEBUG_TYPE_POP_GROUP:			DEBUG_INFO("Type: Pop Group"); break;
	case GL_DEBUG_TYPE_OTHER:				DEBUG_INFO("Type: Other"); break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:			DEBUG_INFO("Severity: high"); break;
	case GL_DEBUG_SEVERITY_MEDIUM:			DEBUG_INFO("Severity: medium"); break;
	case GL_DEBUG_SEVERITY_LOW:				DEBUG_INFO("Severity: low"); break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:	DEBUG_INFO("Severity: notification"); break;
	}
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
		m_sceneManager.ChangeScene(((ChangeSceneEvent&)e).GetTarget());
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
		DEBUG_ERROR("[GLFW] Failed to initialize GLFW");
		glfwTerminate();
		return false;
	}

	DEBUG_INFO("[GLFW] Initialized GLFW {0}", glfwGetVersionString());

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
		DEBUG_ERROR("[GLFW] Failed to create GLFW window");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, this);

	DEBUG_INFO("[GLFW] Window created: {0} ({1}, {2})", title, width, height);

	EventHandler::SetEventCallback(BIND_FUNCTION(Application::EventCallback));

	// Set GLFW callbacks
	glfwSetErrorCallback([](int error, const char* desc)
	{
		DEBUG_ERROR("[GLFW] ({0}) {1}", error, desc);
	});

	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
	{
		auto game = (Application*)glfwGetWindowUserPointer(window);

		game->m_width = width;
		game->m_height = height;

		View::SetScreen((float)width, (float)height);
		glViewport(0, 0, width, height);

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

	// loading glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		DEBUG_ERROR("[GLAD] Failed to initialize GLAD");
		glfwTerminate();
		return false;
	}

	DEBUG_INFO("[GLAD] Initialized");
	DEBUG_INFO("[OpenGL] Version: {0}", glGetString(GL_VERSION));
	DEBUG_INFO("[OpenGL] Vendor: {0}", glGetString(GL_VENDOR));
	DEBUG_INFO("[OpenGL] Renderer: {0}", glGetString(GL_RENDERER));
	DEBUG_INFO("[OpenGL] GLSL Version: {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));

#ifdef _DEBUG
	//Set up opengl debug output
	GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if ((flags & GL_CONTEXT_FLAG_DEBUG_BIT) && (glMajor >= 4 && glMinor >= 3))
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

		DEBUG_INFO("[OpenGL] Debug context created");
	}
#endif

	View::SetScreen((float)m_width, (float)m_height);
	View::Set(0.0f, 0.0f, (float)m_width, (float)m_height);

	// initialize imgui
	//ImGuiRenderer::Init(m_window, ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable);
	ImGuiRenderer::Init(m_window, ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable);

	return true;
}

Application::Application(const std::string& config)
{
	DEBUG_ASSERT(!config.empty(), "Path is emtpy");

	json root = jsonParseFile(config);

	// ---------------| window |------------------------------------------
	std::string title;

	int width;
	int height;

	int glMajor = 4;
	int glMinor = 0;
	
	if (root.find("window") != root.end())
	{
		json window = root.at("window");

		title = jsonToString(window, "title");
		width = jsonToInt(window, "width");
		height = jsonToInt(window, "height");
	}

	// ---------------| gl version |--------------------------------------
	if (root.find("opengl") != root.end())
	{
		json opengl = root.at("opengl");

		glMajor = jsonToInt(opengl, "major");
		glMinor = jsonToInt(opengl, "minor");
	}

	m_running = LoadApplication(title, width, height, glMajor, glMinor);

	// ---------------| load resources |----------------------------------
	std::string resources = jsonToString(root, "resources");

	if (!resources.empty())
		ResourceManager::Load(resources);
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
		Timer::Start();

		if (!m_paused)
			OnUpdate();

		glClear(GL_COLOR_BUFFER_BIT);

		ImGuiRenderer::Start();

		OnRender();

		if (m_debug)
			OnRenderDebug();

		if (m_showImGui)
			OnImGui();

		Primitives::Flush(View::GetMat());
		ImGuiRenderer::Flush(m_window);

		glfwPollEvents();
		EventHandler::Poll();
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
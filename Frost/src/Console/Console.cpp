#include "Console.h"

#include "Core/Graphics.h"
#include "Input/Input.h"

#include <GLFW/glfw3.h>

// TODO: CharLogger
// TODO: show log
// TODO: remove any static

void charCallback(GLFWwindow* window, unsigned int codepoint)
{
	if (Console::s_active != nullptr)
	{
		Console::s_active->m_prompt += static_cast<char>(codepoint);
	}
}

Console* Console::s_active = nullptr;

void Console::execute()
{
	m_interpreter.execute(m_prompt);
	m_log.push_back(m_prompt);
	m_prompt = "";
}

bool Console::Init(GLFWwindow* context)
{
	if (context == nullptr)
	{
		DEBUG_WARN("Console needs a context");
		return false;
	}

	glfwSetCharCallback(context, charCallback);
	
	return true;
}

void Console::Open(Console* console)
{
	s_active = console;
}

void Console::Close(Console* console)
{
	if (console == s_active || console == nullptr)
		s_active = nullptr;
}

void Console::Toggle(Console* console)
{
	if (console == s_active)
		s_active = nullptr;
	else
		s_active = console;
}

void Console::Update()
{
	if (s_active != nullptr)
	{
		if (Input::KeyPressed(KEY_ENTER))
			s_active->execute();

		if (Input::KeyPressed(KEY_BACKSPACE))
			s_active->m_prompt.pop_back();
	}
}

void Console::Render(float x, float y)
{
	if (s_active != nullptr)
	{
		Renderer::RenderString(ResourceManager::GetFont("blocky"), s_active->m_prompt, x, y, Renderer::GetScreenView(), "shader");
	}
}

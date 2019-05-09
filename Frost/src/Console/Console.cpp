#include "Console.h"

#include "Core/Graphics.h"
#include "Input/Input.h"
#include "CharLogger.h"

void Console::execute()
{
	m_interpreter.execute(m_prompt);
	m_log.push_back(m_prompt);
	m_prompt = "";
}

Console::Console()
{
}

void Console::open()
{
	m_open = true;
}

void Console::close()
{
	m_open = false;
	m_prompt.clear();
}

void Console::toggle()
{
	m_open ? close() : open();
}

void Console::update()
{
	if (m_open)
	{
		m_prompt += CharLogger::GetLog();

		if (Input::KeyPressed(KEY_ENTER))
			execute();

		if (Input::KeyPressed(KEY_BACKSPACE) && !m_prompt.empty())
			m_prompt.pop_back();
	}
}

void Console::render(float x, float y) const
{
	if (m_open)
	{
		Renderer::RenderString(ResourceManager::GetFont("blocky"), m_prompt, x, y, Renderer::GetScreenView(), "shader");
	}
}

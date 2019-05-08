#pragma once

#include "Utility/Utils.h"
#include "Utility/Debugger.h"

struct GLFWwindow;

// TODO: write simple interpreter
struct ConsoleInterpreter
{
	virtual void execute(const std::string cmd)
	{
		DEBUG_INFO("Executing: {0}", cmd);
	}
};

class Console
{
private:
	std::vector<std::string> m_log;
	std::string m_prompt;

	ConsoleInterpreter m_interpreter;
	
	static Console* s_active;

	void execute();

	friend void charCallback(GLFWwindow* window, unsigned int codepoint);
public:
	static bool Init(GLFWwindow* context);

	static void Open(Console* console);
	static void Close(Console* console = nullptr);
	static void Toggle(Console* console);

	static void Update();
	static void Render(float x, float y);
};
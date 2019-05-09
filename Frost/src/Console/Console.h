#pragma once

#include "Utility/Utils.h"
#include "Utility/Debugger.h"

struct GLFWwindow;

// TODO: write simple interpreter
// TODO: show log
// TODO: cursor position
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

	bool m_open;

	ConsoleInterpreter m_interpreter;

	void execute();
public:
	Console(/* Interpreter */);

	void open();
	void close();
	void toggle();

	void update();
	void render(float x, float y) const;
};
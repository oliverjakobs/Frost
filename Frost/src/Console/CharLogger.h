#pragma once

#include "Patterns/Singelton.h"

struct GLFWwindow;

class CharLogger : private Singleton<CharLogger>
{
private:
	std::string m_log;

	friend void charCallback(GLFWwindow* window, unsigned int codepoint);
public:
	static bool Init(GLFWwindow* context);

	static std::string GetLog();
};
#include "CharLogger.h"

#include <GLFW/glfw3.h>

void charCallback(GLFWwindow* window, unsigned int codepoint)
{
	CharLogger::Get().m_log += static_cast<char>(codepoint);
}

bool CharLogger::Init(GLFWwindow* context)
{
	if (context == nullptr)
		return false;

	glfwSetCharCallback(context, charCallback);

	return true;
}

std::string CharLogger::GetLog()
{
	std::string temp = Get().m_log;
	Get().m_log.clear();

	return temp;
}

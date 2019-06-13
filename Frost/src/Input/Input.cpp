#include "Input.h"

#include <GLFW\glfw3.h>

bool Input::IsKeyPressed(int keycode)
{
	auto state = glfwGetKey(glfwGetCurrentContext(), keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(int button)
{
	auto state = glfwGetMouseButton(glfwGetCurrentContext(), button);
	return state == GLFW_PRESS;
}

std::pair<float, float> Input::GetMousePosition()
{
	double xpos, ypos;
	glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);

	return { (float)xpos, (float)ypos };
}

float Input::GetMouseX()
{
	auto[x, y] = GetMousePosition();
	return x;
}

float Input::GetMouseY()
{
	auto[x, y] = GetMousePosition();
	return y;
}

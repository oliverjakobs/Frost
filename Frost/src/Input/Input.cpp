#include "Input.h"

#include <GLFW\glfw3.h>

bool Input::KeyPressed(int keycode)
{
	if (keycode > KEY_LAST || keycode == KEY_UNKNOWN)
		return false;

	auto state = glfwGetKey(glfwGetCurrentContext(), keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::KeyReleased(int keycode)
{
	if (keycode > KEY_LAST || keycode == KEY_UNKNOWN)
		return false;

	auto state = glfwGetKey(glfwGetCurrentContext(), keycode);
	return state == GLFW_RELEASE;
}

bool Input::MousePressed(int button)
{
	if (button > MOUSE_BUTTON_LAST || button < MOUSE_BUTTON_1)
		return false;

	auto state = glfwGetMouseButton(glfwGetCurrentContext(), button);
	return state == GLFW_PRESS;
}

bool Input::MouseReleased(int button)
{
	if (button > MOUSE_BUTTON_LAST || button < MOUSE_BUTTON_1)
		return false;

	auto state = glfwGetMouseButton(glfwGetCurrentContext(), button);
	return state == GLFW_RELEASE;
}

glm::vec2 Input::MousePosition()
{
	double xpos, ypos;
	glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);

	return glm::vec2((float)xpos, (float)ypos);
}

float Input::MouseX()
{
	return MousePosition().x;
}

float Input::MouseY()
{
	return MousePosition().y;
}

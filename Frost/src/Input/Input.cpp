#include "Input.hpp"

#include <GLFW\glfw3.h>

bool InputKeyPressed(int keycode)
{
	if (keycode > KEY_LAST || keycode == KEY_UNKNOWN)
		return false;

	int state = glfwGetKey(glfwGetCurrentContext(), keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool InputKeyReleased(int keycode)
{
	if (keycode > KEY_LAST || keycode == KEY_UNKNOWN)
		return false;

	int state = glfwGetKey(glfwGetCurrentContext(), keycode);
	return state == GLFW_RELEASE;
}

bool InputMousePressed(int button)
{
	if (button > MOUSE_BUTTON_LAST || button < MOUSE_BUTTON_1)
		return false;

	int state = glfwGetMouseButton(glfwGetCurrentContext(), button);
	return state == GLFW_PRESS;
}

bool InputMouseReleased(int button)
{
	if (button > MOUSE_BUTTON_LAST || button < MOUSE_BUTTON_1)
		return false;

	int state = glfwGetMouseButton(glfwGetCurrentContext(), button);
	return state == GLFW_RELEASE;
}

glm::vec2 InputMousePosition()
{
	double xpos, ypos;
	glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);

	return glm::vec2((float)xpos, (float)ypos);
}

float InputMouseX()
{
	return InputMousePosition().x;
}

float InputMouseY()
{
	return InputMousePosition().y;
}

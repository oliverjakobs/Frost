#include "Input.h"

#include <GLFW\glfw3.h>

int InputKeyPressed(int keycode)
{
	if (keycode > KEY_LAST || keycode == KEY_UNKNOWN)
		return 0;

	int state = glfwGetKey(glfwGetCurrentContext(), keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

int InputKeyReleased(int keycode)
{
	if (keycode > KEY_LAST || keycode == KEY_UNKNOWN)
		return 0;

	int state = glfwGetKey(glfwGetCurrentContext(), keycode);
	return state == GLFW_RELEASE;
}

int InputMousePressed(int button)
{
	if (button > MOUSE_BUTTON_LAST || button < MOUSE_BUTTON_1)
		return 0;

	int state = glfwGetMouseButton(glfwGetCurrentContext(), button);
	return state == GLFW_PRESS;
}

int InputMouseReleased(int button)
{
	if (button > MOUSE_BUTTON_LAST || button < MOUSE_BUTTON_1)
		return 0;

	int state = glfwGetMouseButton(glfwGetCurrentContext(), button);
	return state == GLFW_RELEASE;
}

void InputMousePosition(float* x, float* y)
{
	double xpos, ypos;
	glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);

	if (x) *x = (float)xpos;
	if (y) *y = (float)ypos;
}

float InputMouseX()
{
	float x;

	InputMousePosition(&x, NULL);

	return x;
}

float InputMouseY()
{
	float y;

	InputMousePosition(NULL, &y);

	return y;
}

vec2 InputMousePositionVec2()
{
	vec2 vec;
	InputMousePosition(&vec.x, &vec.y);
	return vec;
}
#include "Input.h"

// void MinimalUpdate(GLFWwindow* context) { }

int MinimalKeyPressed(Key keycode)
{
	if (keycode > GLFW_KEY_LAST || keycode == GLFW_KEY_UNKNOWN) return 0;

	int state = glfwGetKey(glfwGetCurrentContext(), keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

int MinimalKeyReleased(Key keycode)
{
	if (keycode > GLFW_KEY_LAST || keycode == GLFW_KEY_UNKNOWN) return 0;
	return glfwGetKey(glfwGetCurrentContext(), keycode) == GLFW_RELEASE;
}

int MinimalKeyHit(Key keycode)
{
	return 0;
}

int MinimalKeyDown(Key keycode)
{

	return 0;
}

int MinimalKeyUp(Key keycode)
{

	return 0;
}

int MinimalMousePressed(MouseButton button)
{

	return 0;
}

int MinimalMouseReleased(MouseButton button)
{

	return 0;
}

int MinimalMouseHit(MouseButton button)
{

	return 0;
}

int MinimalMouseDown(MouseButton button)
{

return 0;
}

int MinimalMouseUp(MouseButton button)
{

	return 0;
}

void MinimalCursorPos(float* x, float* y)
{

}

float MinimalCursorX() {
	return 0;
}
float MinimalCursorY() {
	return 0;
}
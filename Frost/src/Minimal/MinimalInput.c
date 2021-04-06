#include "MinimalInput.h"

typedef struct
{
	int state;
	int prev;
} InputState;

static InputState key_states[KEY_LAST + 1];
static InputState mouse_states[MOUSE_BUTTON_LAST + 1];

void InputUpdate(GLFWwindow* context)
{
	for (int i = KEY_SPACE; i <= KEY_LAST; ++i)
	{
		key_states[i].prev = key_states[i].state;
		key_states[i].state = (glfwGetKey(context, i) == GLFW_PRESS);
	}

	for (int i = MOUSE_BUTTON_1; i <= MOUSE_BUTTON_LAST; ++i)
	{
		mouse_states[i].prev = key_states[i].state;
		mouse_states[i].state = (glfwGetMouseButton(context, i) == GLFW_PRESS);
	}
}

int InputKeyPressed(Key keycode)
{
	if (keycode > KEY_LAST || keycode == KEY_UNKNOWN) return 0;

	int state = glfwGetKey(glfwGetCurrentContext(), keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

int InputKeyReleased(Key keycode)
{
	if (keycode > KEY_LAST || keycode == KEY_UNKNOWN) return 0;
	return glfwGetKey(glfwGetCurrentContext(), keycode) == GLFW_RELEASE;
}

int InputKeyHit(Key keycode)
{
	if (keycode > KEY_LAST || keycode == KEY_UNKNOWN) return 0;
	return key_states[keycode].state && !key_states[keycode].prev;
}

int InputKeyDown(Key keycode)
{
	if (keycode > KEY_LAST || keycode == KEY_UNKNOWN) return 0;
	return key_states[keycode].state;
}

int InputKeyUp(Key keycode)
{
	if (keycode > KEY_LAST || keycode == KEY_UNKNOWN) return 0;
	return key_states[keycode].prev && !key_states[keycode].state;
}

int InputMousePressed(MouseButton button)
{
	if (button > MOUSE_BUTTON_LAST || button < MOUSE_BUTTON_1) return 0;
	return glfwGetMouseButton(glfwGetCurrentContext(), button) == GLFW_PRESS;
}

int InputMouseReleased(MouseButton button)
{
	if (button > MOUSE_BUTTON_LAST || button < MOUSE_BUTTON_1) return 0;
	return glfwGetMouseButton(glfwGetCurrentContext(), button) == GLFW_RELEASE;
}

int InputMouseHit(MouseButton button)
{
	if (button > MOUSE_BUTTON_LAST || button == MOUSE_BUTTON_1) return 0;
	return mouse_states[button].state && !mouse_states[button].prev;
}

int InputMouseDown(MouseButton button)
{
	if (button > MOUSE_BUTTON_LAST || button == MOUSE_BUTTON_1) return 0;
	return mouse_states[button].state;
}

int InputMouseUp(MouseButton button)
{
	if (button > MOUSE_BUTTON_LAST || button == MOUSE_BUTTON_1) return 0;
	return mouse_states[button].prev && !mouse_states[button].state;
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

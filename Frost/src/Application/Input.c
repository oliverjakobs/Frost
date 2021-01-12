#include "Input.h"

#include <string.h>

typedef struct
{
	int state;
	int prev;
} InputKeyState;

static InputKeyState key_states[KEY_LAST + 1];

void InputUpdate(GLFWwindow* context)
{
	for (int i = KEY_SPACE; i <= KEY_LAST; ++i)
	{
		key_states[i].prev = key_states[i].state;
		key_states[i].state = (glfwGetKey(context, i) == GLFW_PRESS);
	}
}

int InputKeyPressed(Key keycode)
{
	if (keycode > KEY_LAST || keycode == KEY_UNKNOWN)
		return 0;

	int state = glfwGetKey(glfwGetCurrentContext(), keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

int InputKeyReleased(Key keycode)
{
	if (keycode > KEY_LAST || keycode == KEY_UNKNOWN)
		return 0;

	int state = glfwGetKey(glfwGetCurrentContext(), keycode);
	return state == GLFW_RELEASE;
}

int InputKeyHit(Key keycode)
{
	if (keycode > KEY_LAST || keycode == KEY_UNKNOWN)
		return 0;

	return key_states[keycode].state && !key_states[keycode].prev;
}

int InputKeyDown(Key keycode)
{
	if (keycode > KEY_LAST || keycode == KEY_UNKNOWN)
		return 0;

	return key_states[keycode].state;
}

int InputKeyUp(Key keycode)
{
	if (keycode > KEY_LAST || keycode == KEY_UNKNOWN)
		return 0;

	return key_states[keycode].prev && !key_states[keycode].state;
}

int InputMousePressed(MouseButton button)
{
	if (button > MOUSE_BUTTON_LAST || button < MOUSE_BUTTON_1)
		return 0;

	int state = glfwGetMouseButton(glfwGetCurrentContext(), button);
	return state == GLFW_PRESS;
}

int InputMouseReleased(MouseButton button)
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

static Key InputParseKeyCodeChar(char c)
{
	switch (c)
	{
	case ' ': return KEY_SPACE;
	case '\'': return KEY_APOSTROPHE;
	case ',': return KEY_COMMA;
	case '-': return KEY_MINUS;
	case '.': return KEY_PERIOD;
	case '/': return KEY_SLASH;
	case ';': return KEY_SEMICOLON;
	case '=': return KEY_EQUAL;
	case '[': return KEY_LEFT_BRACKET;
	case '\\': return KEY_BACKSLASH;
	case ']': return KEY_RIGHT_BRACKET;
	case '`': return KEY_GRAVE_ACCENT;

	case '0': return KEY_0;
	case '1': return KEY_1;
	case '2': return KEY_2;
	case '3': return KEY_3;
	case '4': return KEY_4;
	case '5': return KEY_5;
	case '6': return KEY_6;
	case '7': return KEY_7;
	case '8': return KEY_8;
	case '9': return KEY_9;

	case 'a': case 'A': return KEY_A;
	case 'b': case 'B': return KEY_B;
	case 'c': case 'C': return KEY_C;
	case 'd': case 'D': return KEY_D;
	case 'e': case 'E': return KEY_E;
	case 'f': case 'F': return KEY_F;
	case 'g': case 'G': return KEY_G;
	case 'h': case 'H': return KEY_H;
	case 'i': case 'I': return KEY_I;
	case 'j': case 'J': return KEY_J;
	case 'k': case 'K': return KEY_K;
	case 'l': case 'L': return KEY_L;
	case 'm': case 'M': return KEY_M;
	case 'n': case 'N': return KEY_N;
	case 'o': case 'O': return KEY_O;
	case 'p': case 'P': return KEY_P;
	case 'q': case 'Q': return KEY_Q;
	case 'r': case 'R': return KEY_R;
	case 's': case 'S': return KEY_S;
	case 't': case 'T': return KEY_T;
	case 'u': case 'U': return KEY_U;
	case 'v': case 'V': return KEY_V;
	case 'w': case 'W': return KEY_W;
	case 'x': case 'X': return KEY_X;
	case 'y': case 'Y': return KEY_Y;
	case 'z': case 'Z': return KEY_Z;
	}

	return KEY_UNKNOWN;
}

Key InputParseKeyCode(const char* str, size_t size)
{
	if (size == 0) return KEY_UNKNOWN;
	if (size == 1) return InputParseKeyCodeChar(str[0]);

	const char* prefix = "KEY_";
	
	return KEY_UNKNOWN;
}

MouseButton InputParseMouseButton(const char* str, size_t size)
{
	if (size == 0) return MOUSE_BUTTON_UNKNOWN;

	switch (str[0])
	{
	case '1': return MOUSE_BUTTON_1;
	case '2': return MOUSE_BUTTON_2;
	case '3': return MOUSE_BUTTON_3;
	case '4': return MOUSE_BUTTON_4;
	case '5': return MOUSE_BUTTON_5;
	case '6': return MOUSE_BUTTON_6;
	case '7': return MOUSE_BUTTON_7;
	case '8': return MOUSE_BUTTON_8;

	/* TODO case insensitive compare */
	case 'l': 
		if (strcmp(str, "left") == 0) return MOUSE_BUTTON_LEFT;
		break;
	case 'L': 
		if (strcmp(str, "Left") == 0) return MOUSE_BUTTON_LEFT;
		if (strcmp(str, "LEFT") == 0) return MOUSE_BUTTON_LEFT;
		break;
	case 'r':
		if (strcmp(str, "right") == 0) return MOUSE_BUTTON_RIGHT;
		break;
	case 'R':
		if (strcmp(str, "Right") == 0) return MOUSE_BUTTON_RIGHT;
		if (strcmp(str, "RIGHT") == 0) return MOUSE_BUTTON_RIGHT;
		break;
	case 'm':
		if (strcmp(str, "middle") == 0) return MOUSE_BUTTON_MIDDLE;
		break;
	case 'M':
		if (strcmp(str, "Middle") == 0) return MOUSE_BUTTON_MIDDLE;
		if (strcmp(str, "MIDDLE") == 0) return MOUSE_BUTTON_MIDDLE;
		break;
	}
	return MOUSE_BUTTON_UNKNOWN;
}

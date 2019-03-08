#include "Input.h"

#include <GLFW\glfw3.h>

namespace sb
{
	Input::Input()
	{
		for (int i = 0; i < GLFW_KEY_LAST; i++)
		{
			m_keys[i] = { false, false };
		}

		for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
		{
			m_mouseButtons[i] = { false, false };
		}

		m_mousePos = glm::vec2();
	}

	Input::~Input()
	{
	}

	void sb::Input::SetKeyState(unsigned int key, bool pressed, bool prev)
	{
		Get().m_keys[key] = { pressed, prev };
	}

	void Input::OnUpdate(GLFWwindow* context)
	{
		for (auto& key : Get().m_keys)
		{
			key.second.prev = key.second.pressed;
			key.second.pressed = (glfwGetKey(context, key.first) == GLFW_PRESS);
		}

		for (auto& button : Get().m_mouseButtons)
		{
			button.second.prev = button.second.pressed;
			button.second.pressed = (glfwGetKey(context, button.first) == GLFW_PRESS);
		}

		double x, y;
		glfwGetCursorPos(context, &x, &y);
		
		Get().m_mousePos.x = static_cast<float>(x);
		Get().m_mousePos.y = static_cast<float>(y);
	}

	bool Input::KeyDown(unsigned int key)
	{
		if (key >= GLFW_KEY_LAST || key == GLFW_KEY_UNKNOWN)
			return false;

		return Get().m_keys[key].pressed;
	}

	bool Input::KeyPressed(unsigned int key)
	{
		if (key >= GLFW_KEY_LAST || key == GLFW_KEY_UNKNOWN)
			return false;
						
		return Get().m_keys[key].pressed && !Get().m_keys[key].prev;
	}

	bool Input::KeyReleased(unsigned int key)
	{
		if (key >= GLFW_KEY_LAST || key == GLFW_KEY_UNKNOWN)
			return false;

		return (Get().m_keys[key].prev && !Get().m_keys[key].pressed);
	}

	bool Input::MouseButtonDown(unsigned int button)
	{
		if (button >= GLFW_MOUSE_BUTTON_LAST || button == GLFW_KEY_UNKNOWN)
			return false;

		return Get().m_mouseButtons[button].pressed;
	}

	bool Input::MouseButtonPressed(unsigned int button)
	{
		if (button >= GLFW_MOUSE_BUTTON_LAST || button == GLFW_KEY_UNKNOWN)
			return false;

		return Get().m_mouseButtons[button].pressed && !Get().m_mouseButtons[button].prev;
	}

	bool Input::MouseButtonReleased(unsigned int button)
	{
		if (button >= GLFW_MOUSE_BUTTON_LAST || button == GLFW_KEY_UNKNOWN)
			return false;

		return Get().m_mouseButtons[button].pressed;
	}

	glm::vec2 Input::MousePosition()
	{
		return Get().m_mousePos;
	}

	KeyState Input::GetKeyState(unsigned int key)
	{
		return Get().m_keys[key];
	}

	KeyState Input::GetMouseButtonState(unsigned int key)
	{
		return Get().m_mouseButtons[key];
	}
}
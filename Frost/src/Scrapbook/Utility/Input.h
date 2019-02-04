#pragma once

#include "Singelton.h"

#include <sstream>
#include <map>

#include "Scrapbook/Maths.h"

namespace sb
{
	struct KeyState
	{
		bool pressed;
		bool prev;

		std::string toString() const
		{
			std::stringstream ss;
			ss << "KeyState: " << pressed << ", " << prev;
			return ss.str();
		}
	};

	class Input : private Singleton<Input>
	{
	private:
		std::map<unsigned int, KeyState> m_keys;
		std::map<unsigned int, KeyState> m_mouseButtons;

		glm::vec2 m_mousePos;
	public:
		Input();
		~Input();

		static void SetKeyState(unsigned int key, bool pressed, bool prev);
		static void OnUpdate();

		static bool KeyDown(unsigned int key);
		static bool KeyPressed(unsigned int key);
		static bool KeyReleased(unsigned int key);

		static bool MouseButtonDown(unsigned int button);
		static bool MouseButtonPressed(unsigned int button);
		static bool MouseButtonReleased(unsigned int button);

		static glm::vec2 MousePosition();

		static KeyState GetKeyState(unsigned int key);
		static KeyState GetMouseButtonState(unsigned int key);
	};
}
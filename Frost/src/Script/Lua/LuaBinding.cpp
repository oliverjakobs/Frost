#include "LuaBinding.h"

#include "String/StringUtils.h"
#include "Input/Input.h"

unsigned int LuaInput::GetKeyCode(const std::string& key)
{
	if (stringCompare(key, "SPACE"))
		return KEY_SPACE;

	return key.front();
}

bool LuaInput::KeyPressed(const std::string& key)
{
	return Input::KeyPressed(GetKeyCode(key));
}

bool LuaInput::KeyReleased(const std::string & key)
{
	return Input::KeyReleased(GetKeyCode(key));
}
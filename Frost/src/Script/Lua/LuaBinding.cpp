#include "LuaBinding.h"

#include "String/StringUtils.h"
#include "Input/Input.h"


void LuaBinding::CreateState(sol::state lua)
{
	lua.open_libraries(sol::lib::base);

	lua.new_usertype<glm::vec2>("Vec2",
		sol::constructors<glm::vec2(), glm::vec2(float, float)>(),
		"x", &glm::vec2::x,
		"y", &glm::vec2::y
		);

	// Input
	lua.new_usertype<LuaInput>("Input",
		sol::constructors<LuaInput()>(),
		"KeyPressed", &LuaInput::KeyPressed,
		"KeyReleased", &LuaInput::KeyReleased
		);

	// functions
	lua.set_function("GetVelocity", []() { });
	lua.set_function("CollidesBottom", []() { });
	lua.set_function("Drop", []() { });
	lua.set_function("SetVelocity", [](const glm::vec2&) { });
}

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
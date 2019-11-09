#include "LuaBinding.h"

#include "Input/Input.h"

#include "Obelisk/Obelisk.h"

int LuaInput::GetKeyCode(const std::string& key)
{
	if (obelisk::StringCompare(key, "SPACE"))
		return KEY_SPACE;

	return key.front();
}

int LuaInput::GetButtonCode(const std::string& button)
{
	if (obelisk::StringCompare(button, "LEFT"))
		return MOUSE_BUTTON_LEFT;
	if (obelisk::StringCompare(button, "RIGHT"))
		return MOUSE_BUTTON_RIGHT;
	if (obelisk::StringCompare(button, "MIDDLE"))
		return MOUSE_BUTTON_MIDDLE;

	return MOUSE_BUTTON_LAST + 1;
}

bool LuaInput::KeyPressed(const std::string& key)
{
	return Input::KeyPressed(GetKeyCode(key));
}

bool LuaInput::KeyReleased(const std::string& key)
{
	return Input::KeyReleased(GetKeyCode(key));
}

bool LuaInput::MousePressed(const std::string& button)
{
	return Input::MousePressed(GetButtonCode(button));
}

bool LuaInput::MouseReleased(const std::string& button)
{
	return Input::MouseReleased(GetButtonCode(button));
}

glm::vec2 LuaInput::MousePosition()
{
	return Input::MousePosition();
}

float LuaInput::MouseX()
{
	return Input::MouseX();
}

float LuaInput::MouseY()
{
	return Input::MouseY();
}

void LoadLuaState(sol::state& lua)
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
		"KeyPressed",		&LuaInput::KeyPressed,
		"KeyReleased",		&LuaInput::KeyReleased,
		"MousePressed",		&LuaInput::MousePressed,
		"MouseReleased",	&LuaInput::MouseReleased,
		"MousePosition",	&LuaInput::MousePosition,
		"MouseX",			&LuaInput::MouseX,
		"MouseY",			&LuaInput::MouseY
		);

	// Entity
	lua.new_usertype<LuaEntity>("Entity",
		sol::constructors<LuaEntity(Entity*)>(),
		// base
		"SetPosition",		&LuaEntity::SetPosition,
		"SetDimension",		&LuaEntity::SetDimension,
		"SetDirection",		&LuaEntity::SetDirection,
		"GetPosition",		&LuaEntity::GetPosition,
		"GetDimension",		&LuaEntity::GetDimension,
		"GetDirection",		&LuaEntity::GetDirection,
		// physics
		"SetVelocity",		&LuaEntity::SetVelocity,
		"GetVelocity",		&LuaEntity::GetVelocity,
		"CollidesBottom",	&LuaEntity::CollidesBottom,
		"CollidesTop",		&LuaEntity::CollidesTop,
		"CollidesLeft",		&LuaEntity::CollidesLeft,
		"CollidesRight",	&LuaEntity::CollidesRight,
		"Drop",				&LuaEntity::Drop,
		// graphics
		"PlayAnimation",	&LuaEntity::PlayAnimation,
		"SetRenderFlip",	&LuaEntity::SetRenderFlip,
		// view
		"CenterView",		&LuaEntity::CenterView
		);

	// Scene
	lua.new_usertype<LuaScenes>("Scenes",
		sol::constructors<LuaScenes()>(),
		"Change",			&LuaScenes::Change
		);
}

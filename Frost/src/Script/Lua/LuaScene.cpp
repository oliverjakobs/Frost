#include "LuaScene.h"

#include "Event/EventHandler.h"

void LuaScenes::Change(const std::string& name)
{
	EventHandler::Throw<ChangeSceneEvent>(name);
}

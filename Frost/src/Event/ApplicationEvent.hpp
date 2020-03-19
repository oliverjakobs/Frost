#pragma once

#include "Event.hpp"

#include <string>

struct ChangeSceneEvent : public Event
{
	std::string TargetScene;

	ChangeSceneEvent(const std::string& target) : Event(EventType::ChangeScene), TargetScene(target) {}
};

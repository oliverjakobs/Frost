#pragma once

#include "Event.h"

class ChangeSceneEvent : public Event
{
private:
	std::string m_targetScene;

public:
	ChangeSceneEvent(const std::string& target) : m_targetScene(target) {}

	inline std::string GetTarget() const { return m_targetScene; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "ChangeSceneEvent: " << m_targetScene;
		return ss.str();
	}

	EVENT_CLASS_TYPE(ChangeScene);
	EVENT_CLASS_CATEGORY(EventCategoryApplication);
};

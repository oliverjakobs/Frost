#include "Api.h"

static std::function<void(ignisErrorLevel level, const std::string&)> s_errorCallback;

void ignisSetErrorCallback(std::function<void(ignisErrorLevel level, const std::string&)> callback)
{
	s_errorCallback = callback;
}

void _ignisErrorCallback(ignisErrorLevel level, const std::string& desc)
{
	s_errorCallback(level, desc);
}

glm::vec4 ignis::BlendColor(const glm::vec4& color, float alpha)
{
	return glm::vec4(color.r, color.g, color.b, alpha);
}

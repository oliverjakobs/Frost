#include "JSONParser.h"

glm::vec2 jsonToVec2(nlohmann::json j, const std::string& x, const std::string& y, const glm::vec2& default)
{
	glm::vec2 vec = default;

	if (j.find(x) != j.end())
		vec.x = j.at(x);

	if (j.find(y) != j.end())
		vec.y = j.at(y);

	return vec;
}

float jsonToFloat(nlohmann::json j, const std::string& f, float default)
{
	float value = default;

	if (j.find(f) != j.end())
		value = j.at(f);

	return value;
}

int jsonToInt(nlohmann::json j, const std::string& i, int default)
{
	int value = default;

	if (j.find(i) != j.end())
		value = j.at(i);

	return value;
}

std::string jsonToString(nlohmann::json j, const std::string& s, const std::string& default)
{
	std::string value = default;

	if (j.find(s) != j.end())
		value = j.at(s);

	return value;
}

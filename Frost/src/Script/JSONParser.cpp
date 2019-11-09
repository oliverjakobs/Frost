#include "JSONParser.h"

#include "File/FileReader.h"

#include "Obelisk/Debugger.h"

json jsonParseFile(const std::string& path)
{
	std::string jsonString = ReadFile(path.c_str());

	if (!jsonString.empty())
	{
		try
		{
			return json::parse(jsonString);
		}
		catch (json::parse_error& e)
		{
			OBELISK_ERROR("%s", e.what());
		}
	}

	return nlohmann::json();
}

glm::vec2 jsonToVec2(json j, const std::string& name, const glm::vec2& default)
{
	glm::vec2 vec = default;

	if (j.find(name) != j.end())
	{
		json jVec = j.at(name);

		if (jVec.is_array() && jVec.size() == 2)
		{
			vec.x = jVec.at(0);
			vec.y = jVec.at(1);
		}
		else
		{
			OBELISK_WARN("%s could not be parsed to vec2", name.c_str());
		}
	}

	return vec;
}

float jsonToFloat(json j, const std::string& name, float default)
{
	float value = default;

	if (j.find(name) != j.end())
		value = j.at(name);

	return value;
}

int jsonToInt(json j, const std::string& name, int default)
{
	int value = default;

	if (j.find(name) != j.end())
		value = j.at(name);

	return value;
}

std::string jsonToString(json j, const std::string& name, const std::string& default)
{
	std::string value = default;

	if (j.find(name) != j.end())
		value = j.at(name);

	return value;
}

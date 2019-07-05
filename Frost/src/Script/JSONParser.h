#pragma once

#include <string>
#include <glm.hpp>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

json jsonParseFile(const std::string& path);

glm::vec2 jsonToVec2(json j, const std::string& name, const glm::vec2& default = glm::vec2());
float jsonToFloat(json j, const std::string& name, float default = 0.0f);
int jsonToInt(json j, const std::string& name, int default = 0.0f);

std::string jsonToString(json j, const std::string& s, const std::string& default = "");
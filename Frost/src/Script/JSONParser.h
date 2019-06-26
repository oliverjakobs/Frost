#pragma once

#include <string>
#include <glm/glm.hpp>

#include <nlohmann/json.hpp>

glm::vec2 jsonToVec2(nlohmann::json j, const std::string& x, const std::string& y, const glm::vec2& default = glm::vec2());
float jsonToFloat(nlohmann::json j, const std::string& f, float default = 0.0f);
int jsonToInt(nlohmann::json j, const std::string& i, int default = 0.0f);

std::string jsonToString(nlohmann::json j, const std::string& s, const std::string& default = "");
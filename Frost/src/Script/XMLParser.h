#pragma once

#include <tinyxml2.h>

#include <string>
#include <glm/glm.hpp>

using namespace tinyxml2;

class XMLParser
{
private:
	bool m_valid;

	XMLDocument m_doc;

public:
	XMLParser(const std::string& xmlPath);

	bool IsValid() const;
	bool HasRoot(const std::string& name) const;

	XMLElement* GetRoot();

	static glm::vec2 to_vec2(XMLElement* element, const glm::vec2& default = glm::vec2());
};
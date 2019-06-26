#include "XMLParser.h"

#include "Log/Logger.h"

XMLParser::XMLParser(const std::string& xmlPath)
{
	XMLError result = m_doc.LoadFile(xmlPath.c_str());

	if (result == XML_SUCCESS)
	{
		m_valid = true;
	}
	else
	{
		// TODO: better output
		DEBUG_WARN("Failed to load file ({0}): {1}", xmlPath, result);
		m_valid = false;
	}
}

bool XMLParser::IsValid() const
{
	return m_valid;
}

bool XMLParser::HasRoot(const std::string& name) const
{
	return m_valid && (m_doc.FirstChildElement(name.c_str()) != nullptr);
}

XMLElement* XMLParser::GetRoot()
{
	if (m_valid)
		return m_doc.RootElement();

	return nullptr;
}

glm::vec2 XMLParser::to_vec2(XMLElement* element, const glm::vec2& default)
{
	if (element != nullptr)
		return glm::vec2(element->FloatAttribute("x"), element->FloatAttribute("y"));

	return default;
}

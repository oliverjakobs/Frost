#include "XMLParser.h"

#include "Utility/Debugger.h"

bool XMLParser::Load(const std::string& file)
{
	m_doc.LoadFile(file.c_str());

	if (m_doc.Error())
	{
		DEBUG_ERROR("tinyxml2::XMLError: {0}", m_doc.ErrorStr());
		return false;
	}

	return true;
}

void XMLParser::Clear()
{
	m_doc.Clear();
}

XMLElement XMLParser::Root()
{
	return { m_doc.RootElement() };
}

void XMLParser::Print() const
{
	m_doc.Print();
}

XMLElement XMLElement::Get(const std::string& name) const
{
	return { elem->FirstChildElement(c_str_null(name)) };
}

std::optional<std::string> XMLElement::Attribute(const std::string& name)
{
	auto a = elem->Attribute(name.c_str());

	if (a != nullptr)
		return std::optional<std::string>(a);

	return std::optional<std::string>();
}

std::string XMLElement::Text() const
{
	return elem->GetText();
}

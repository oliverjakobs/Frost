#pragma once

#include "Utility/Utils.h"
#include "tinyxml/tinyxml2.h"

#include <optional>

struct XMLElement
{
	tinyxml2::XMLElement* elem;

	inline bool Empty() const
	{
		return elem == nullptr;
	}

	operator bool() const
	{
		return elem != nullptr;
	}

	XMLElement Get(const std::string& name = "") const;
	std::optional<std::string> Attribute(const std::string& name);
	std::string Text() const;

	// only supports int, unsigned int, int64_t, bool, double and float
	template<typename Type>
	std::optional<Type> Attribute(const std::string& name)
	{
		Type value;
		auto result = elem->QueryAttribute(name.c_str(), &value);

		if (result != tinyxml2::XML_SUCCESS)
		{
			DEBUG_ERROR("tinyxml2::XMLError: {0}", tinyxml2::XMLDocument::ErrorIDToName(result));
			return std::optional<Type>();
		}

		return std::optional<Type>(value);
	}
};

class XMLParser
{
private:
	tinyxml2::XMLDocument m_doc;

public:
	XMLParser() = default;

	bool Load(const std::string& file);
	void Clear();

	XMLElement Root();
	void Print() const;
};
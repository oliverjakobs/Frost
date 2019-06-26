#include "ResourceManager.h"

#include "Log/Logger.h"

#include "Script/XMLParser.h"

void ResourceManager::AddShader(const std::string& name, Shader* shader)
{
	DEBUG_ASSERT(!name.empty(), "Shader name missing");

	Get().m_shaders[name] = unique_ptr<Shader>(shader);
}

Shader* ResourceManager::GetShader(const std::string& name)
{
	try
	{
		return Get().m_shaders.at(name).get();
	}
	catch (std::out_of_range)
	{
		DEBUG_WARN("No such shader: {0}", name);
		return nullptr;
	}
}

void ResourceManager::AddImage(const std::string& name, Image* image)
{
	DEBUG_ASSERT(!name.empty(), "Image name missing");

	Get().m_images[name] = unique_ptr<Image>(image);
}

Image* ResourceManager::GetImage(const std::string& name)
{
	try
	{
		return Get().m_images.at(name).get();
	}
	catch (std::out_of_range)
	{
		DEBUG_WARN("No such image: {0}", name);
		return nullptr;
	}
}

void ResourceManager::AddFont(const std::string& name, Font* font)
{
	DEBUG_ASSERT(!name.empty(), "Font name missing");

	Get().m_fonts[name] = unique_ptr<Font>(font);
}

Font* ResourceManager::GetFont(const std::string& name)
{
	try
	{
		return Get().m_fonts.at(name).get();
	}
	catch (std::out_of_range)
	{
		DEBUG_WARN("No such font: {0}", name);
		return nullptr;
	}
}

void ResourceManager::Load(const char* xmlPath)
{
	XMLParser xml(xmlPath);

	DEBUG_ASSERT(xml.HasRoot("Resources"), "Root not found ({0})", xmlPath)

	// shaders 
	for (XMLElement* elem = xml.GetRoot()->FirstChildElement("Shader"); elem != nullptr; elem = elem->NextSiblingElement("Shader"))
	{
		// TODO: error checking
		std::string name = elem->Attribute("name");
		std::string vert = elem->Attribute("vert");
		std::string frag = elem->Attribute("frag");

		AddShader(name, new Shader(vert, frag));
	}

	// images 
	for (XMLElement* elem = xml.GetRoot()->FirstChildElement("Image"); elem != nullptr; elem = elem->NextSiblingElement("Image"))
	{
		std::string name = elem->Attribute("name");
		std::string src = elem->Attribute("src");
		float width = elem->FloatAttribute("w");
		float height = elem->FloatAttribute("h");
		unsigned int rows = elem->IntAttribute("row", 1);
		unsigned int columns = elem->IntAttribute("col", 1);

		AddImage(name, new Image(src, width, height, rows, columns));
	}

	// Fonts 
	for (XMLElement* elem = xml.GetRoot()->FirstChildElement("Font"); elem != nullptr; elem = elem->NextSiblingElement("Font"))
	{
		std::string name = elem->Attribute("name");
		std::string src = elem->Attribute("src");
		float width = elem->FloatAttribute("w");
		float height = elem->FloatAttribute("h");
		float spacing = elem->FloatAttribute("spacing");

		AddFont(name, new BitmapFont(src, width, height, spacing));
	}
}
#include "ResourceManager.h"

#include "Log/Logger.h"

#include "Script/XMLParser.h"
#include "Font/BitmapFont.h"

#include "Script/JSONParser.h"

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

void ResourceManager::Load(const std::string& path)
{
	DEBUG_ASSERT(!path.empty(), "Path is emtpy");

	json root = jsonParseFile(path);

	// shaders 
	if (root.find("shaders") != root.end())
	{
		for (auto&[name, shader] : root.at("shaders").items())
		{
			std::string vert = jsonToString(shader, "vert");
			std::string frag = jsonToString(shader, "frag");

			AddShader(name, new Shader(vert, frag));
		}
	}

	// images 
	if (root.find("images") != root.end())
	{
		for (auto&[name, image] : root.at("images").items())
		{
			std::string src = jsonToString(image, "src");
			float width = jsonToFloat(image, "width");
			float height = jsonToFloat(image, "height");
			unsigned int rows = jsonToInt(image, "rows", 1);
			unsigned int columns = jsonToInt(image, "columns", 1);

			AddImage(name, new Image(src, width, height, rows, columns));
		}
	}

	// Fonts 
	if (root.find("fonts") != root.end())
	{
		for (auto&[name, image] : root.at("fonts").items())
		{
			std::string src = jsonToString(image, "src");
			float width = jsonToFloat(image, "width");
			float height = jsonToFloat(image, "height");
			float spacing = jsonToFloat(image, "spacing");

			AddFont(name, new BitmapFont(src, width, height, spacing));
		}
	}
}
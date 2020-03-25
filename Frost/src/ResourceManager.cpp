#include "ResourceManager.hpp"

#include "Obelisk/Debugger.hpp"

#include "json/TemplateLoader.hpp"

ResourceManager::ResourceManager(const std::string& path)
{
	TemplateLoader::LoadResourceManager(this, path);
}

ResourceManager::~ResourceManager()
{
	for (auto& [name, texture] : m_textures)
		ignisDestroyTexture(texture);
}

void ResourceManager::AddTexture(const std::string& name, const std::string& path, int rows, int columns)
{
	IgnisTexture* texture = (IgnisTexture*)malloc(sizeof(IgnisTexture));

	if (ignisCreateTexture(texture, path.c_str(), rows, columns, true, NULL))
	{
		const auto [it, success] = m_textures.insert({ name, texture });

		if (!success)
		{
			OBELISK_WARN("[res] Texture \"%s\" already in use", name.c_str());
			ignisDestroyTexture(texture);
		}
	}
}

IgnisTexture* ResourceManager::GetTexture(const std::string& name) const
{
	auto it = m_textures.find(name);
	if (it != m_textures.end())
	{
		return it->second;
	}

	return nullptr;
}

void ResourceManager::AddFont(const std::string& name, const std::string& path, float size)
{
	IgnisFont* font = (IgnisFont*)malloc(sizeof(IgnisFont));

	if (ignisLoadFont(font, path.c_str(), size))
	{
		const auto [it, success] = m_fonts.insert({ name, font });

		if (!success)
		{
			OBELISK_WARN("[res] Font \"%s\" already in use", name.c_str());
			ignisDeleteFont(font);
		}
	}
}

IgnisFont* ResourceManager::GetFont(const std::string& name) const
{
	auto it = m_fonts.find(name);
	if (it != m_fonts.end())
	{
		return it->second;
	}

	return nullptr;
}

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
	ignis_texture* texture = ignisCreateTexture(path.c_str(), rows, columns, true);

	if (texture)
	{
		const auto [it, success] = m_textures.insert({ name, texture });

		if (!success)
		{
			OBELISK_WARN("[res] Texture \"%s\" already in use", name.c_str());
			ignisDestroyTexture(texture);
		}
	}
}

ignis_texture* ResourceManager::GetTexture(const std::string& name) const
{
	auto it = m_textures.find(name);
	if (it != m_textures.end())
	{
		return it->second;
	}

	return nullptr;
}

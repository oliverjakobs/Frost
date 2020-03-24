#include "ResourceManager.hpp"

#include "Obelisk/Debugger.hpp"

void ResourceManager::Destroy()
{
	for (auto& [name, texture] : m_textures)
		ignisDestroyTexture(texture);
}

void ResourceManager::AddTexture(const std::string& name, ignis_texture* texture)
{
	const auto [it, success] = m_textures.insert({ name, texture });

	if (!success)
		OBELISK_WARN("[res] Texture \"%s\" already in use", name.c_str());
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

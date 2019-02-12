#include "ResourceManager.h"

#include "Scrapbook/Utility.h"

namespace sb
{
	ResourceManager* ResourceManager::s_instance = nullptr;

	void ResourceManager::Load()
	{
		s_instance = new ResourceManager();
	}

	void ResourceManager::Free()
	{
		for (auto& s : s_instance->m_shaders)
		{
			SAFE_DELETE(s.second);
		}

		s_instance->m_shaders.clear();
		s_instance = nullptr;
	}

	void ResourceManager::AddShader(const std::string& name, Shader* shader)
	{
		s_instance->m_shaders[name] = shader;
	}

	Shader* ResourceManager::GetShader(const std::string& name)
	{
		try
		{
			return s_instance->m_shaders.at(name);
		}
		catch (std::out_of_range)
		{
			DEBUG_MESSAGE("No such shader: " << name);
			return nullptr;
		}
	}
}
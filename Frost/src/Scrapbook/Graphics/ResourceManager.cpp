#include "ResourceManager.h"

#include "Scrapbook/Utility.h"

namespace sb
{
	void ResourceManager::AddShader(const std::string& name, Shader* shader)
	{
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
			DEBUG_MESSAGE("No such shader: " << name);
			return nullptr;
		}
	}

	void ResourceManager::AddImage(const std::string& name, Image* image)
	{
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
			DEBUG_MESSAGE("No such image: " << name);
			return nullptr;
		}
	}
}
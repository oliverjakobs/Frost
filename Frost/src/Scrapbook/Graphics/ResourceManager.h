#pragma once

#include "Image.h"

namespace sb
{
	class ResourceManager : private Singleton<ResourceManager>
	{
	private:
		std::map<std::string, shared_ptr<Shader>> m_shaders;
		std::map<std::string, shared_ptr<Image>> m_images;

	public:
		static void Free() { Get().m_shaders.clear(); }

		static void AddShader(const std::string& name, Shader* shader);
		static shared_ptr<Shader> GetShader(const std::string& name);

		static void AddImage(const std::string& name, Image* image);
		static shared_ptr<Image> GetImage(const std::string& name);
	};
}

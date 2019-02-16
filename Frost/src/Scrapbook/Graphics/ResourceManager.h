#pragma once

#include "Image.h"

namespace sb
{
	class ResourceManager : private Singleton<ResourceManager>
	{
	private:
		std::map<std::string, unique_ptr<Shader>> m_shaders;
		std::map<std::string, unique_ptr<Image>> m_images;

	public:
		// TODO: Rescource creation only through here
		static void AddShader(const std::string& name, Shader* shader);
		static Shader* GetShader(const std::string& name);

		static void AddImage(const std::string& name, Image* image);
		static Image* GetImage(const std::string& name);
	};
}

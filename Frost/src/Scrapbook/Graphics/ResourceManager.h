#pragma once

#include "Image.h"

namespace sb
{
	class ResourceManager
	{
	private:
		static ResourceManager* s_instance;

		std::map<std::string, Shader*> m_shaders;
		std::map<std::string, Image*> m_images;
		//std::map<std::string, Shader*> m_spriteSheets;
	public:
		static void Load();
		static void Free();

		static void AddShader(const std::string& name, Shader* shader);
		static Shader* GetShader(const std::string& name);

		static void AddImage(const std::string& name, Image* shader);
		static Image* GetImage(const std::string& name);
	};
}

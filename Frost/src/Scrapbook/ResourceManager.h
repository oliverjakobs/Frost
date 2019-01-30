#pragma once

#include "Scrapbook\Shader.h"

#include <map>

namespace sb
{
	class ResourceManager
	{
	private:
		static ResourceManager* s_instance;

		std::map<std::string, Shader*> m_shaders;
	public:
		static void Load();

		static void AddShader(const std::string& name, Shader* shader);
		static Shader* GetShader(const std::string& name);
	};
}

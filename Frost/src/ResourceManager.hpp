#pragma once

#include "Ignis/Ignis.h"

#include <map>
#include <string>

class ResourceManager
{
private:
	std::map<std::string, ignis_texture*> m_textures;

public:
	ResourceManager(const std::string& path);
	~ResourceManager();

	void AddTexture(const std::string& name, const std::string& path, int rows, int columns);
	ignis_texture* GetTexture(const std::string& name) const;
};
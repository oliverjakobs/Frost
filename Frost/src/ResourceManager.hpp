#pragma once

#include "Ignis/Ignis.h"

#include <map>
#include <string>

class ResourceManager
{
private:
	std::map<std::string, ignis_texture*> m_textures;

public:
	void Destroy();

	void AddTexture(const std::string& name, ignis_texture* texture);
	ignis_texture* GetTexture(const std::string& name) const;
};
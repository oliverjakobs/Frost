#pragma once

#include "Ignis/Ignis.h"

#include <map>
#include <string>

class ResourceManager
{
private:
	std::map<std::string, IgnisTexture*> m_textures;
	std::map<std::string, IgnisFont*> m_fonts;

public:
	ResourceManager(const std::string& path);
	~ResourceManager();

	void AddTexture(const std::string& name, const std::string& path, int rows, int columns);
	IgnisTexture* GetTexture(const std::string& name) const;

	void AddFont(const std::string& name, const std::string& path, float size);
	IgnisFont* GetFont(const std::string& name) const;
};